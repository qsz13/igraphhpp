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

igraphhpp_test_relative_path = "tests/"
compiler_message_path = "compiler_message/"
last_status_fn = "svn_update_last_status.txt"
compiler_result_fn = 'compiling_result.html'
outfn = "tmp.exe"

options = " -Wall -Wno-unknown-pragmas -Wno-attributes -fno-strict-aliasing -O2 "
links = " -I%(igraphhpp_path)s -I/opt/local/include -ligraph -lgsl -lgslcblas -L/opt/local/lib "
iofiles = " -o %(outfn)s %(infn)s 2>> %(msgfn)s "

compiler_command = {
"g++-4.4_gnu++0x" : " g++-4.4 -std=gnu++0x" + options + links + iofiles,
"g++-4.4" : " g++-4.4 " + options + links + iofiles,
"g++-4.3" : " g++-4.3 " + options + links + iofiles,
"g++-4.2" : " g++-4.2 " + options + links + iofiles,
"g++-4.1" : " g++-4.1 " + options + links + iofiles,
"llvm-g++-4.2" : " llvm-g++ " + options + links + iofiles,
"msvc2008" : " /home/share/msvc/bin/cl.exe /Tp%(infn)s /wd4068 /I/home/share/msvc/include /I%(igraphhpp_path)s /I/usr/include/ /link /LIBPATH:/home/share/msvc/lib /Fe%(outfn)s  1> %(msgfn)s "
}


htmlcode = '''<html><head><title>igraphhpp regression test</title>
<style type="text/css">
<!--
body {font-size: 1em; font-family: sans-serif;}
.fail {background-color: red; text-align:center;}
.pass {background-color: lightgreen; text-align:center;}
.warn {background-color: rgb(255,233,0); text-align:center;}
--></style>

<body>
<h1 style="text-align: center;">igraphhpp regression test</h1>

Generated at %(time)s, %(revision)s  Tested on ubuntu 9.04 i386<br/>
Documentation <a href="html/index.html">here</a>.
<font class="fail">FAIL</font>: compiled with some error. <font class="pass">PASS</font>: pass with no warning. <font class="warn">PASS</font>: pass with some warning.<br/>
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


def test(igraphhpp_path,igraphhpp_test_path,name,commands,compiler_message_path):
    compile_result = []
    for title,base_cmd in commands.iteritems():
        msgfn = compiler_message_path + title + name + ".txt"
        infn = igraphhpp_test_path+name
        cmd = base_cmd % {"igraphhpp_path":igraphhpp_path, "outfn":outfn, "infn":infn, "msgfn":msgfn};
        os.system("echo 'Test cases: (" + title + "," + name + ")  start at "
        + time.strftime("%a, %d %b %Y %H:%M:%S (local time,HK)", time.localtime())
        + "\n"+cmd+"' > "+msgfn);
        return_code = os.system(cmd)
        compile_result += [[msgfn,return_code]]
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
    igraphhpp_path = os.path.abspath(os.sys.argv[2])
    igraphhpp_test_path = igraphhpp_path + "/" + igraphhpp_test_relative_path
    
    os.chdir(working_dir)
    
    old_status = ""
    new_status = "\n\n\n"
    if(len(os.sys.argv)==3):
        try:
            old_status = open(last_status_fn, "r").read()
        except:
            pass
        os.system("svn update " + igraphhpp_path + " > "+last_status_fn)
        new_status = open(last_status_fn, "r").read()
        if(old_status==new_status):
            exit(0)

    starting_time = time.strftime("%a, %d %b %Y %H:%M:%S (local time,HK)", time.localtime())

    #os.system("echo 'regression test for igraphhpp. The following are the compiled result in the (compiler,file) pair' > "+compiler_message_fn);
    header = "<tr><td></td>"
    for title,base_cmd in compiler_command.iteritems():
        header += "<td>" + title + "</td>";
    header += "</tr>"

    filelists = []
    for fn in listdir(igraphhpp_test_path):
        if os.path.splitext(fn)[1] == ".cpp":
            filelists += [fn]
    
    results = {}
    for infn in filelists:
        results[infn] = test(igraphhpp_path,igraphhpp_test_path,infn,compiler_command,compiler_message_path)

    s = ""
    for fn,row in results.iteritems():
        s += "<tr><td>" + fn + "</td>";
        for res in row:
            if(res[1]>0):
                s += "<td class='fail' title='"+str(res[1])+"'><a href='" + res[0] + "'>FAIL</a></td>"
            else:
                has_warning = open(res[0],"r").read().lower().find("warning:") != -1
                if(has_warning):
                    s += "<td class='warn'><a href='" + res[0] + "'>PASS</a></td>"
                else:
                    s += "<td class='pass'><a href='" + res[0] + "'>PASS</a></td>"
        s += "</tr>"
    
    f = open(compiler_result_fn, 'w')
    f.write(htmlcode % {
    "time" : starting_time, "revision" : new_status.split("\n")[-2],
    "header" : header, "result" : s}
    )
    
    os.system("doxygen")
    
    #TODO: create better output
    #TODO: check the correctness of the running result


