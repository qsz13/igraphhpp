/*

randist.hpp ... GSL Random Number Distributions C++ wrapper

Copyright (C) 2009  KennyTM~

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef GSL_RANDIST_HPP
#define GSL_RANDIST_HPP

#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>
#include <gsl/cpp/rng.hpp>
#include <exception>
#include <stdexcept>
#include <utility>
#include <cstring>
#include <tempobj.hpp>

namespace gsl {	
	class Random;
	
	class UnimplementedException : public std::exception {
	private:
		const char* errmsg;
	public:
		virtual const char* what() const throw() { return errmsg; }
		UnimplementedException (const char* errmsg_) throw() : errmsg(errmsg_) {};	
	};
	
#pragma mark -
	
	/// An abstract interface of a random distribution.
	template <typename T = double>
#if 0
	x;	// fix those smart IDEs not recognizing default template parameters.
#endif
	class IRandomDistribution {
	protected:
		static const gsl_rng* raw(const Random& rangen) throw() { return rangen._; }
	public:
		/// Get a sample of the distribution using the specified random number generator.
		/// Note: If the distribution returns an array, you must delete[] it after you've finished using it.
		///       You should not ::std::free() it (that includes moving the array to an igraph Vector), 
		//        because the two systems are not compatible in general.
		virtual T get(const Random& rangen) const throw() = 0;
		virtual double pdf(const T) const throw(UnimplementedException) { throw UnimplementedException("pdf unimplemented for this distribution."); }
		virtual double P(const T) const throw(UnimplementedException) { throw UnimplementedException("P unimplemented for this distribution."); }
		virtual double Q(const T) const throw(UnimplementedException) { throw UnimplementedException("Q unimplemented for this distribution."); }
		virtual double Pinv(const T) const throw(UnimplementedException) { throw UnimplementedException("Pinv unimplemented for this distribution."); }
		virtual double Qinv(const T) const throw(UnimplementedException) { throw UnimplementedException("Qinv unimplemented for this distribution."); }
	};

#pragma mark -
	
#define XXINTRNL_RANDIST_DECLARE_GET__METHOD_WITH_TYPE(typ, xxname, ...) typ get(const Random& rangen) const throw() { return gsl_ran_##xxname (raw(rangen), ##__VA_ARGS__); }
#define XXINTRNL_RANDIST_DECLARE_PDF__METHOD_WITH_TYPE(typ, xxname, ...) double get(const typ x) const throw() { return gsl_ran_##xxname##_pdf(x, ##__VA_ARGS__); }
#define XXINTRNL_RANDIST_DECLARE_P____METHOD_WITH_TYPE(typ, xxname, ...) double P(const typ x) const throw() { return gsl_cdf_##xxname##_P(x, ##__VA_ARGS__); }
#define XXINTRNL_RANDIST_DECLARE_Q____METHOD_WITH_TYPE(typ, xxname, ...) double Q(const typ x) const throw() { return gsl_cdf_##xxname##_Q(x, ##__VA_ARGS__); }
#define XXINTRNL_RANDIST_DECLARE_GET__METHOD(...) XXINTRNL_RANDIST_DECLARE_GET__METHOD_WITH_TYPE(double, __VA_ARGS__)
#define XXINTRNL_RANDIST_DECLARE_PDF__METHOD(...) XXINTRNL_RANDIST_DECLARE_PDF__METHOD_WITH_TYPE(double, __VA_ARGS__)
#define XXINTRNL_RANDIST_DECLARE_P____METHOD(...) XXINTRNL_RANDIST_DECLARE_P____METHOD_WITH_TYPE(double, __VA_ARGS__)
#define XXINTRNL_RANDIST_DECLARE_Q____METHOD(...) XXINTRNL_RANDIST_DECLARE_Q____METHOD_WITH_TYPE(double, __VA_ARGS__)
#define XXINTRNL_RANDIST_DECLARE_PINV_METHOD(xxname, ...) double Pinv(const double x) const throw() { return gsl_cdf_##xxname##_Pinv(x, ##__VA_ARGS__); }
#define XXINTRNL_RANDIST_DECLARE_QINV_METHOD(xxname, ...) double Qinv(const double x) const throw() { return gsl_cdf_##xxname##_Qinv(x, ##__VA_ARGS__); }

#pragma mark -

	class GaussianDistribution : public IRandomDistribution<double> {
	public:
		enum Method {
			BoxMuller,
			Ziggurat,
			RatioMethod
		};
	private:
		double sigma;
		double (*sampler)(const gsl_rng*, double);
	public:
		GaussianDistribution(double sigma_ = 1.0, Method method_ = Ziggurat) : sigma(sigma_) {
			switch (method_) {
				case Ziggurat:
					sampler = &gsl_ran_gaussian_ziggurat;
					break;
				case RatioMethod:
					sampler = &gsl_ran_gaussian_ratio_method;
					break;
				default:
					sampler = &gsl_ran_gaussian;
					break;
			}
		}
		double get(const Random& rangen) const throw() { return sampler(raw(rangen), sigma); }
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD(gaussian, sigma);
		XXINTRNL_RANDIST_DECLARE_P____METHOD(gaussian, sigma);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD(gaussian, sigma);
		XXINTRNL_RANDIST_DECLARE_PINV_METHOD(gaussian, sigma);
		XXINTRNL_RANDIST_DECLARE_QINV_METHOD(gaussian, sigma);
	};
	
#pragma mark -
	
	class GaussianTailDistribution : public IRandomDistribution<double> {
	private:
		double a;
		double sigma;
	public:
		GaussianTailDistribution(double a_, double sigma_ = 1.0) : a(a_), sigma(sigma_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(gaussian_tail, a, sigma);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD(gaussian_tail, a, sigma);
	};
	
#pragma mark -
	
	// This can't inherit from IRandomDistribution because it returns 2 numbers
	class BivariateGaussianDistribution : public IRandomDistribution< ::std::pair<double,double> > {
	private:
		double sigma_x, sigma_y, rho;
	public:
		BivariateGaussianDistribution(double sigma_x_, double sigma_y_, double rho_) : sigma_x(sigma_x_), sigma_y(sigma_y_), rho(rho_) {}
		::std::pair<double,double> get(const Random& rangen) const throw() {
			double x, y;
			gsl_ran_bivariate_gaussian(raw(rangen), sigma_x, sigma_y, rho, &x, &y);
			return ::std::pair<double,double>(x, y);
		}
		double pdf(double x, double y) const throw() { return gsl_ran_bivariate_gaussian_pdf(x, y, sigma_x, sigma_y, rho); }
		double pdf(const ::std::pair<double,double>& v) const throw() { return pdf(v.first, v.second); }
	};
	
#pragma mark -
	
	class LaplaceDistribution : public IRandomDistribution<double> {
	private:
		double a;
	public:
		LaplaceDistribution(double a_) : a(a_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(laplace, a);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD(laplace, a);
		XXINTRNL_RANDIST_DECLARE_P____METHOD(laplace, a);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD(laplace, a);
		XXINTRNL_RANDIST_DECLARE_PINV_METHOD(laplace, a);
		XXINTRNL_RANDIST_DECLARE_QINV_METHOD(laplace, a);
	};
	
#pragma mark -
	
	class ExponentialPowerDistribution : public IRandomDistribution<double> {
	private:
		double a, b;
	public:
		ExponentialPowerDistribution(double a_, double b_) : a(a_), b(b_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(exppow, a, b);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD(exppow, a, b);
		XXINTRNL_RANDIST_DECLARE_P____METHOD(exppow, a, b);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD(exppow, a, b);
	};
	
#pragma mark -
	
	class CauchyDistribution : public IRandomDistribution<double> {
	private:
		double a;
	public:
		CauchyDistribution(double a_) : a(a_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(cauchy, a);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD(cauchy, a);
		XXINTRNL_RANDIST_DECLARE_P____METHOD(cauchy, a);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD(cauchy, a);
		XXINTRNL_RANDIST_DECLARE_PINV_METHOD(cauchy, a);
		XXINTRNL_RANDIST_DECLARE_QINV_METHOD(cauchy, a);
	};
	
#pragma mark -
	
	class RayleighDistribution : public IRandomDistribution<double> {
	private:
		double sigma;
	public:
		RayleighDistribution(double sigma_) : sigma(sigma_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(rayleigh, sigma);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD(rayleigh, sigma);
		XXINTRNL_RANDIST_DECLARE_P____METHOD(rayleigh, sigma);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD(rayleigh, sigma);
		XXINTRNL_RANDIST_DECLARE_PINV_METHOD(rayleigh, sigma);
		XXINTRNL_RANDIST_DECLARE_QINV_METHOD(rayleigh, sigma);
	};
	
#pragma mark -
	
	class RayleighTailDistribution : public IRandomDistribution<double> {
	private:
		double a, sigma;
	public:
		RayleighTailDistribution(double a_, double sigma_) : a(a_), sigma(sigma_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(rayleigh_tail, a, sigma);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD(rayleigh_tail, a, sigma);
	};
	
#pragma mark -
	
	class LandauDistribution : public IRandomDistribution<double> {
	public:
		LandauDistribution() {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(landau);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD(landau);
	};
	
#pragma mark -
	
	class LevyDistribution : public IRandomDistribution<double> {
	private:
		double c, alpha;
	public:
		LevyDistribution(double c_, double alpha_) : c(c_), alpha(alpha_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(levy, c, alpha);
	};
	
#pragma mark -
	
	class LevySkewDistribution : public IRandomDistribution<double> {
	private:
		double c, alpha, beta;
	public:
		LevySkewDistribution(double c_, double alpha_, double beta_) : c(c_), alpha(alpha_), beta(beta_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(levy_skew, c, alpha, beta);
	};
	
#pragma mark -
	
	class GammaDistribution : public IRandomDistribution<double> {
	private:
		double a, b;
		double (*sampler)(const gsl_rng*, double, double);
	public:
		enum Method {
			MarsagliaTsang,
			Knuth
		};
		GammaDistribution(double a_, double b_, Method method = MarsagliaTsang) : a(a_), b(b_) {
			if (method == Knuth)
				sampler = &gsl_ran_gamma_knuth;
			else
				sampler = &gsl_ran_gamma;
		}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(gamma, a, b);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD(gamma, a, b);
		XXINTRNL_RANDIST_DECLARE_P____METHOD(gamma, a, b);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD(gamma, a, b);
		XXINTRNL_RANDIST_DECLARE_PINV_METHOD(gamma, a, b);
		XXINTRNL_RANDIST_DECLARE_QINV_METHOD(gamma, a, b);
	};
	
#pragma mark -
	
	class FlatDistribution : public IRandomDistribution<double> {
	private:
		double a, b;
	public:
		FlatDistribution(double a_, double b_) : a(a_), b(b_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(flat, a, b);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD(flat, a, b);
		XXINTRNL_RANDIST_DECLARE_P____METHOD(flat, a, b);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD(flat, a, b);
		XXINTRNL_RANDIST_DECLARE_PINV_METHOD(flat, a, b);
		XXINTRNL_RANDIST_DECLARE_QINV_METHOD(flat, a, b);
	};
	typedef FlatDistribution UniformDistribution;
		
#pragma mark -
	
	class LognormalDistribution : public IRandomDistribution<double> {
	private:
		double zeta, sigma;
	public:
		LognormalDistribution(double zeta_, double sigma_) : zeta(zeta_), sigma(sigma_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(lognormal, zeta, sigma);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD(lognormal, zeta, sigma);
		XXINTRNL_RANDIST_DECLARE_P____METHOD(lognormal, zeta, sigma);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD(lognormal, zeta, sigma);
		XXINTRNL_RANDIST_DECLARE_PINV_METHOD(lognormal, zeta, sigma);
		XXINTRNL_RANDIST_DECLARE_QINV_METHOD(lognormal, zeta, sigma);
	};
	
#pragma mark -
	
	class ChiSquaredDistribution : public IRandomDistribution<double> {
	private:
		double nu;
	public:
		ChiSquaredDistribution(double nu_) : nu(nu_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(chisq, nu);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD(chisq, nu);
		XXINTRNL_RANDIST_DECLARE_P____METHOD(chisq, nu);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD(chisq, nu);
		XXINTRNL_RANDIST_DECLARE_PINV_METHOD(chisq, nu);
		XXINTRNL_RANDIST_DECLARE_QINV_METHOD(chisq, nu);
	};
	
#pragma mark -
	
	class FDistribution : public IRandomDistribution<double> {
	private:
		double nu1, nu2;
	public:
		FDistribution(double nu1_, double nu2_) : nu1(nu1_), nu2(nu2_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(fdist, nu1, nu2);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD(fdist, nu1, nu2);
		XXINTRNL_RANDIST_DECLARE_P____METHOD(fdist, nu1, nu2);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD(fdist, nu1, nu2);
		XXINTRNL_RANDIST_DECLARE_PINV_METHOD(fdist, nu1, nu2);
		XXINTRNL_RANDIST_DECLARE_QINV_METHOD(fdist, nu1, nu2);
	};
	
#pragma mark -
	
	class tDistribution : public IRandomDistribution<double> {
	private:
		double nu;
	public:
		tDistribution(double nu_) : nu(nu_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(tdist, nu);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD(tdist, nu);
		XXINTRNL_RANDIST_DECLARE_P____METHOD(tdist, nu);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD(tdist, nu);
		XXINTRNL_RANDIST_DECLARE_PINV_METHOD(tdist, nu);
		XXINTRNL_RANDIST_DECLARE_QINV_METHOD(tdist, nu);		
	};
	
#pragma mark -
	
	class BetaDistribution : public IRandomDistribution<double> {
	private:
		double a, b;
	public:
		BetaDistribution(double a_, double b_) : a(a_), b(b_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(beta, a, b);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD(beta, a, b);
		XXINTRNL_RANDIST_DECLARE_P____METHOD(beta, a, b);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD(beta, a, b);
		XXINTRNL_RANDIST_DECLARE_PINV_METHOD(beta, a, b);
		XXINTRNL_RANDIST_DECLARE_QINV_METHOD(beta, a, b);
	};
	
#pragma mark -
	
	class LogisticDistribution : public IRandomDistribution<double> {
	private:
		double a;
	public:
		LogisticDistribution(double a_) : a(a_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(logistic, a);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD(logistic, a);
		XXINTRNL_RANDIST_DECLARE_P____METHOD(logistic, a);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD(logistic, a);
		XXINTRNL_RANDIST_DECLARE_PINV_METHOD(logistic, a);
		XXINTRNL_RANDIST_DECLARE_QINV_METHOD(logistic, a);
	};
	
#pragma mark -
	
	class ParetoDistribution : public IRandomDistribution<double> {
	private:
		double a, b;
	public:
		ParetoDistribution(double a_, double b_) : a(a_), b(b_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(pareto, a, b);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD(pareto, a, b);
		XXINTRNL_RANDIST_DECLARE_P____METHOD(pareto, a, b);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD(pareto, a, b);
		XXINTRNL_RANDIST_DECLARE_PINV_METHOD(pareto, a, b);
		XXINTRNL_RANDIST_DECLARE_QINV_METHOD(pareto, a, b);
	};
	
#pragma mark -
	
	class SphericalVectorDistribution : IRandomDistribution<double*> {
	private:
		size_t n;
		bool altsampler;
	public:
		enum Method {
			Default,
			TrigMethod
		};
		SphericalVectorDistribution(size_t dimensions, Method method = Default) throw() : n(dimensions), altsampler(method == TrigMethod) {}
		double* get(const Random& rangen) const throw() {
			double* resvec = new double[n];
			const gsl_rng* rg = raw(rangen);
			if (n == 2) {
				if (!altsampler)
					gsl_ran_dir_2d(rg, resvec, resvec+1);
				else
					gsl_ran_dir_2d_trig_method(rg, resvec, resvec+1);
			} else if (n == 3)
				gsl_ran_dir_3d(rg, resvec, resvec+1, resvec+2);
			else
				gsl_ran_dir_nd(rg, n, resvec);
			return resvec;
		}
	};
	
#pragma mark -
	
	class WeibullDistribution : public IRandomDistribution<double> {
	private:
		double a, b;
	public:
		WeibullDistribution(double a_, double b_) : a(a_), b(b_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(weibull, a, b);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD(weibull, a, b);
		XXINTRNL_RANDIST_DECLARE_P____METHOD(weibull, a, b);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD(weibull, a, b);
		XXINTRNL_RANDIST_DECLARE_PINV_METHOD(weibull, a, b);
		XXINTRNL_RANDIST_DECLARE_QINV_METHOD(weibull, a, b);
	};
	
#pragma mark -
	
	class Type1GumbelDistribution : public IRandomDistribution<double> {
	private:
		double a, b;
	public:
		Type1GumbelDistribution(double a_, double b_) : a(a_), b(b_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(gumbel1, a, b);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD(gumbel1, a, b);
		XXINTRNL_RANDIST_DECLARE_P____METHOD(gumbel1, a, b);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD(gumbel1, a, b);
		XXINTRNL_RANDIST_DECLARE_PINV_METHOD(gumbel1, a, b);
		XXINTRNL_RANDIST_DECLARE_QINV_METHOD(gumbel1, a, b);
	};
	
	class Type2GumbelDistribution : public IRandomDistribution<double> {
	private:
		double a, b;
	public:
		Type2GumbelDistribution(double a_, double b_) : a(a_), b(b_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD(gumbel2, a, b);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD(gumbel2, a, b);
		XXINTRNL_RANDIST_DECLARE_P____METHOD(gumbel2, a, b);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD(gumbel2, a, b);
		XXINTRNL_RANDIST_DECLARE_PINV_METHOD(gumbel2, a, b);
		XXINTRNL_RANDIST_DECLARE_QINV_METHOD(gumbel2, a, b);
	};
	
#pragma mark -
	
	class DirichletDistribution : IRandomDistribution<double*> {
	private:
		size_t K;
		double* alpha;
	public:
		MEMORY_MANAGER_INTERFACE(DirichletDistribution);
		DirichletDistribution(size_t K_, const double alpha_[]) : K(K_), alpha(new double[K_]) { ::std::memcpy(alpha, alpha_, sizeof(double)*K_); }
		double* get(const Random& rangen) const throw() { 
			double* theta = new double[K];
			gsl_ran_dirichlet(raw(rangen), K, alpha, theta);
			return theta; 
		}
		double pdf(const double theta[]) const throw() { return gsl_ran_dirichlet_pdf(K, alpha, theta); }
		double lnpdf(const double theta[]) const throw() { return gsl_ran_dirichlet_lnpdf(K, alpha, theta); }
	};
	MEMORY_MANAGER_INTERFACE_EX(DirichletDistribution);
	MEMORY_MANAGER_IMPLEMENTATION(DirichletDistribution);
	IMPLEMENT_COPY_METHOD(DirichletDistribution) {
		K = other.K;
		alpha = new double[K];
		::std::memcpy(alpha, other.alpha, sizeof(double)*K);
	}
	IMPLEMENT_MOVE_METHOD(DirichletDistribution) {
		K = ::std::move(other.K);
		alpha = ::std::move(other.alpha);
	}
	IMPLEMENT_DEALLOC_METHOD(DirichletDistribution) {
		delete[] alpha;
	}
	
#pragma mark -
	
	class GeneralDiscreteDistribution : IRandomDistribution<size_t> {
	private:
		gsl_ran_discrete_t* g;
	public:
		GeneralDiscreteDistribution(size_t K, const double p[]) : g(gsl_ran_discrete_preproc(K, p)) {}
		MEMORY_MANAGER_INTERFACE_NO_COPYING(GeneralDiscreteDistribution);
		XXINTRNL_RANDIST_DECLARE_GET__METHOD_WITH_TYPE(size_t, discrete, g);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD_WITH_TYPE(size_t, discrete, g);
	};
	MEMORY_MANAGER_INTERFACE_EX(GeneralDiscreteDistribution);
	MEMORY_MANAGER_IMPLEMENTATION_NO_COPYING(GeneralDiscreteDistribution);
	IMPLEMENT_MOVE_METHOD(GeneralDiscreteDistribution) {
		g = ::std::move(other.g);
	}
	IMPLEMENT_DEALLOC_METHOD(GeneralDiscreteDistribution) {
		gsl_ran_discrete_free(g);
	}
	
#pragma mark -
	
	class PoissonDistribution : public IRandomDistribution<unsigned> {
	private:
		double p;
	public:
		PoissonDistribution(double p_) : p(p_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD_WITH_TYPE(unsigned, poisson, p);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD_WITH_TYPE(unsigned, poisson, p);
		XXINTRNL_RANDIST_DECLARE_P____METHOD_WITH_TYPE(unsigned, poisson, p);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD_WITH_TYPE(unsigned, poisson, p);
	};
	
#pragma mark -
	
	class BernoulliDistribution : public IRandomDistribution<unsigned> {
	private:
		double p;
	public:
		BernoulliDistribution(double p_) : p(p_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD_WITH_TYPE(unsigned, bernoulli, p);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD_WITH_TYPE(unsigned, bernoulli, p);
	};
	
#pragma mark -
	
	class BinomialDistribution : public IRandomDistribution<unsigned> {
	private:
		double p;
		unsigned n;
	public:
		BinomialDistribution(double p_, unsigned n_) : p(p_), n(n_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD_WITH_TYPE(unsigned, binomial, p, n);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD_WITH_TYPE(unsigned, binomial, p, n);
		XXINTRNL_RANDIST_DECLARE_P____METHOD_WITH_TYPE(unsigned, binomial, p, n);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD_WITH_TYPE(unsigned, binomial, p, n);
	};
	
#pragma mark -
	
	class MultinomialDistribution : IRandomDistribution<unsigned*> {
	private:
		size_t K;
		unsigned N;
		double* p;
	public:
		MEMORY_MANAGER_INTERFACE(MultinomialDistribution);
		MultinomialDistribution(size_t K_, unsigned N_, const double* p_) : K(K_), N(N_), p(new double[K_]) { ::std::memcpy(p, p_, sizeof(double)*K_); }
		unsigned* get(const Random& rangen) const throw() { 
			unsigned* res = new unsigned[K];
			gsl_ran_multinomial(raw(rangen), K, N, p, res);
			return res;
		}
		double pdf(const unsigned n[]) const throw() { return gsl_ran_multinomial_pdf(K, p, n); }
		double lnpdf(const unsigned n[]) const throw() { return gsl_ran_multinomial_pdf(K, p, n); }
	};
	MEMORY_MANAGER_INTERFACE_EX(MultinomialDistribution);
	MEMORY_MANAGER_IMPLEMENTATION(MultinomialDistribution);
	IMPLEMENT_COPY_METHOD(MultinomialDistribution) {
		K = other.K;
		N = other.N;
		p = new double[K];
		::std::memcpy(p, other.p, sizeof(double)*K);
	}
	IMPLEMENT_MOVE_METHOD(MultinomialDistribution) {
		K = ::std::move(other.K);
		N = ::std::move(other.N);
		p = ::std::move(other.p);
	}
	IMPLEMENT_DEALLOC_METHOD(MultinomialDistribution) {
		delete[] p;
	}
	
#pragma mark -
	
	class NegativeBinomialDistribution : public IRandomDistribution<unsigned> {
	private:
		double p, n;
	public:
		NegativeBinomialDistribution(double p_, double n_) : p(p_), n(n_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD_WITH_TYPE(unsigned, negative_binomial, p, n);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD_WITH_TYPE(unsigned, negative_binomial, p, n);
		XXINTRNL_RANDIST_DECLARE_P____METHOD_WITH_TYPE(unsigned, negative_binomial, p, n);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD_WITH_TYPE(unsigned, negative_binomial, p, n);
	};
	
#pragma mark -
	
	class PascalDistribution : public IRandomDistribution<unsigned> {
	private:
		double p;
		unsigned n;
	public:
		PascalDistribution(double p_, unsigned n_) : p(p_), n(n_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD_WITH_TYPE(unsigned, pascal, p, n);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD_WITH_TYPE(unsigned, pascal, p, n);
		XXINTRNL_RANDIST_DECLARE_P____METHOD_WITH_TYPE(unsigned, pascal, p, n);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD_WITH_TYPE(unsigned, pascal, p, n);
	};
	
#pragma mark -
	
	class GeometricDistribution : public IRandomDistribution<unsigned> {
	private:
		double p;
	public:
		GeometricDistribution(double p_) : p(p_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD_WITH_TYPE(unsigned, geometric, p);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD_WITH_TYPE(unsigned, geometric, p);
		XXINTRNL_RANDIST_DECLARE_P____METHOD_WITH_TYPE(unsigned, geometric, p);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD_WITH_TYPE(unsigned, geometric, p);
	};
	
#pragma mark -
	
	class HypergeometricDistribution : public IRandomDistribution<unsigned> {
	private:
		unsigned n1, n2, t;
	public:
		HypergeometricDistribution(unsigned n1_, unsigned n2_, unsigned t_) : n1(n1_), n2(n2_), t(t_) {}
		XXINTRNL_RANDIST_DECLARE_GET__METHOD_WITH_TYPE(unsigned, hypergeometric, n1, n2, t);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD_WITH_TYPE(unsigned, hypergeometric, n1, n2, t);
		XXINTRNL_RANDIST_DECLARE_P____METHOD_WITH_TYPE(unsigned, hypergeometric, n1, n2, t);
		XXINTRNL_RANDIST_DECLARE_Q____METHOD_WITH_TYPE(unsigned, hypergeometric, n1, n2, t);
	};
	
#pragma mark -
	
	class LogarithmicDistribution : public IRandomDistribution<unsigned> {
	private:
		double p;
	public:
		XXINTRNL_RANDIST_DECLARE_GET__METHOD_WITH_TYPE(unsigned, logarithmic, p);
		XXINTRNL_RANDIST_DECLARE_PDF__METHOD_WITH_TYPE(unsigned, logarithmic, p);
	};
	
#undef XXINTRNL_RANDIST_DECLARE_GET__METHOD
#undef XXINTRNL_RANDIST_DECLARE_PDF__METHOD
#undef XXINTRNL_RANDIST_DECLARE_P____METHOD
#undef XXINTRNL_RANDIST_DECLARE_Q____METHOD
#undef XXINTRNL_RANDIST_DECLARE_PINV_METHOD
#undef XXINTRNL_RANDIST_DECLARE_QINV_METHOD
	
}

#endif