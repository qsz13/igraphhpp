#!/usr/bin/env python
'''
Regression test of the igraphhpp(http://code.google.com/p/igraphhpp)

Copyright (C) 2009  Hon Wai, LAU <lau65536@gmail.com>

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
'''

import os
import time

CXX0X = "-std=gnu++0x"
igraph_test_relative_path = "tests/"
outfn = "tmp.exe"
last_status_fn = "svn_update_last_status.txt"
compiler_result_fn = 'compiling_result.html'
compiler_message_fn = "compiler_message.txt"

options = " -Wall -Wno-unknown-pragmas -Wno-attributes -fno-strict-aliasing -O2 "
links = " -I%s -I/opt/local/include -ligraph -lgsl -lgslcblas -L/opt/local/lib "
iofiles = " -o %s %s 2>>  " + compiler_message_fn


compiler_command = {
"g++-4.4_gnu++0x" : " g++-4.4 " + CXX0X + options + links + iofiles,
"g++-4.3_gnu++0x" : " g++-4.3 " + CXX0X + options + links + iofiles,
"g++-4.4" : " g++-4.4 " + options + links + iofiles,
"g++-4.3" : " g++-4.3 " + options + links + iofiles,
"g++-4.2" : " g++-4.2 " + options + links + iofiles,
"g++-4.1" : " g++-4.1 " + options + links + iofiles,
"llvm-g++-4.2" : " llvm-g++ " + options + links + iofiles,
}


htmlcode = '''<html><head><title>igraphhpp regression test</title>
<style type="text/css">
<!--
body {font-size: 1em; font-family: sans-serif;}
td.fail {background-color: red; text-align:center;}
td.pass {background-color: lightgreen; text-align:center;}
--></style>

<body>
<h1 style="text-align: center;">igraphhpp regression test</h1>

Generated at %(time)s, %(revision)s  Tested on ubuntu 9.04 i386<br/>
Compiler message <a href="%(compiler_message_fn)s">here<a>. green means pass, red means fail<br/>
<br/>
<div><table cellspacing="1" cellpadding="5" border="0" style="">
  %(header)s
  %(result)s
</table></div>

</body></html>
'''

def listdir(root, path=""):
    files = []
    try:
        for file in os.listdir(os.path.join(root, path)):
            pathname = os.path.join(path, file)
            if os.path.isdir(os.path.join(root, pathname)):
                files.extend(listdir(root, pathname))
            else:
                files.append(pathname)
    except OSError:
        pass
    return files


def test(igraph_path,infn,commands):
    compile_result = []
    for title,base_cmd in commands.iteritems():
        cmd = base_cmd % (igraph_path,outfn,infn);
        os.system("echo '\n\n\n***\n"+cmd+"' >> "+compiler_message_fn);
        return_code = os.system(cmd)
        compile_result += [return_code]
        print infn, title, return_code
        if return_code == 0:
            os.remove(outfn)
    return compile_result




if __name__ == "__main__":
    if(len(os.sys.argv)!=3 and len(os.sys.argv)!=4):
        print '''Usage: working_directory igraphhpp_root_directory [force]\n
        if force is used, no svn update and preform regression test directly 
        e.g. python regression_test.py ./ ../ force        
        '''
        exit(0)
    working_dir = os.sys.argv[1]
    igraph_path = os.path.abspath(os.sys.argv[2])
    igraph_test_path = igraph_path + "/" + igraph_test_relative_path
    
    os.chdir(working_dir)
    
    old_status = ""
    new_status = "\n\n\n"
    if(len(os.sys.argv)==3):
        try:
            old_status = open(last_status_fn, "r").read()
        except:
            pass
        os.system("svn update " + igraph_path + " > "+last_status_fn)
        new_status = open(last_status_fn, "r").read()
        if(old_status==new_status):
            exit(0)

    starting_time = time.strftime("%a, %d %b %Y %H:%M:%S (local time,HK)", time.localtime())

    os.system("echo 'igraphhpp regression test' > "+compiler_message_fn);
    header = "<tr><td></td>"
    for title,base_cmd in compiler_command.iteritems():
        header += "<td>" + title + "</td>";
    header += "</tr>"

    filelists = []
    for fn in listdir(igraph_test_path):
        if os.path.splitext(fn)[1] == ".cpp":
            filelists += [fn]
    
    results = {}
    for infn in filelists:
        results[infn] = test(igraph_path,igraph_test_path+infn,compiler_command)

    s = ""
    for fn,row in results.iteritems():
        s += "<tr><td>" + fn + "</td>";
        for code in row:
            if(code>0):
                s += "<td class='fail'>" + str(code) + "</td>"
            else:
                s += "<td class='pass'>PASS</td>"
        s += "</tr>"
    
    f = open(compiler_result_fn, 'w')
    f.write(htmlcode % {
    "time" : starting_time, "revision" : new_status.split("\n")[-2],
    "compiler_message_fn" : compiler_message_fn,
    "header" : header, "result" : s}
    )
    
    #TODO: create better output
    #TODO: check the correctness of the running result

