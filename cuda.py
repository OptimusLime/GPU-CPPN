#!/usr/bin/env python
# encoding: utf-8
# Thomas Nagy, 2010

"cuda"

import os

import Task
from TaskGen import extension
import ccroot

cuda_str = '${NVCC} ${CUDAFLAGS} ${_CCINCFLAGS} ${_CCDEFFLAGS} ${SRC} -o ${TGT}'
cls = Task.simple_task_type('cuda', cuda_str, 'GREEN', ext_out='.o', ext_in='.cu', shell=False)
cls.scan = ccroot.scan

@extension(['.cu', '.cuda'])
def c_hook(self, node):
	tsk = self.create_task('cuda', node, node.change_ext('.o'))
	self.compiled_tasks.append(tsk)
	return tsk

def detect(conf):
	cudaloc = conf.find_program('nvcc', var='NVCC', mandatory=True)
	conf.env.LIB_CUDA = ['cuda', 'cudart']
	conf.env.LIBPATH_CUDA = [cudaloc[:-8] + "lib"]
	conf.env.INCLUDES_CUDA = [cudaloc[:-8] + "include"]


# , ≈c_preproc
# import ccroot, c_preproc
# import extension

# class cuda(Task.Task):
# 	run_str = '${NVCC} ${SRC} -o ${TGT}'
# 	color   = 'PINK'

# @extension('.cu')
# def c_hook(self, node):
# 	return create_compiled_task(self, 'cuda', node)

# cuda_str = '${NVCC} -c ${SRC} -o ${TGT}'
# cls = Task.simple_task_type('cuda', cuda_str, 'PINK', ext_out='.o', ext_in='.c', shell=False)
# cls.scan = ccroot.scan

# @extension(['.cu', '.cuda'])
# def c_hook(self, node):
# 	tsk = self.create_task('cuda', node, node.change_ext('.o'))
# 	self.compiled_tasks.append(tsk)
# 	return tsk


# def set_options(opt):
# 	opt.tool_options('compiler_cxx')

# def configure(conf):
# 	conf.find_program('nvcc', var='NVCC', mandatory=True)
# 	conf.find_cuda_libs(conf)
# 	# conf.check_tool('cuda', tooldir='.')

# def create_compiled_task(self, name, node):
# 	"""
# 	Create the compilation task: c, cxx, asm, etc. The output node is created automatically (object file with a typical **.o** extension).
# 	The task is appended to the list *compiled_tasks* which is then used by :py:func:`waflib.Tools.ccroot.apply_link`
# 	:param name: name of the task class
# 	:type name: string
# 	:param node: the file to compile
# 	:type node: :py:class:`waflib.Node.Node`
# 	:return: The task created
# 	:rtype: :py:class:`waflib.Task.Task`
# 	"""
# 	# out = '%s' % (node.name)
# 	task = self.create_task(name, node, node.change_ext('.o'))

# 	print task
# 	# self.source.extend(task.outputs)
	
# 	try:
# 		self.compiled_tasks.append(task)
# 	except AttributeError:
# 		self.compiled_tasks = [task]
# 	return task

# @conf
# def find_cuda_libs(self):
# 	"""
# 	find the cuda include and library folders

# 	use ctx.program(source='main.c', target='app', use='CUDA CUDART')
# 	"""

# 	if not self.env.NVCC:
# 		self.fatal('check for nvcc first')

# 	print '\n' + self.env.NVCC[:-8] + '\n'

# 	_includes = self.env.NVCC[:-8] + "include"
# 	_libpath = []
# 	_libpath.append(self.env.NVCC[:-8] + "lib")

# 	# this should not raise any error
# 	self.check_cxx(header='cuda.h', lib='cuda', libpath=_libpath, includes=_includes)
# 	self.check_cxx(header='cuda.h', lib='cudart', libpath=_libpath, includes=_includes)

# 	print '\n Assumed CUDA Locations: ' + _includes + '\n + Lib Paths: ' + _libpath[0] + '\n'
