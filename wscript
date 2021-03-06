import Options
from os import unlink, symlink, popen
from os.path import exists 

import os

srcdir = '.'
blddir = 'build'
VERSION = '0.0.1'

def set_options(opt):
  opt.tool_options('compiler_cxx')

def configure(conf):
  conf.check_tool('compiler_cxx')
  conf.check_tool('node_addon')

def build(bld):
  obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
  obj.target = 'redis_shared'
  obj.source = ["redis_shared.cc"]
  obj.includes = "./src"
  obj.linkflags  = [os.getcwd() + "/src/redis_server.a", 
	os.getcwd() + "/deps/jemalloc/lib/libjemalloc.a"]
 
