from enum import Enum

import logging
import sys
import os


SHADER_RESOURCES_SOURCE_H_TEMPLATE = """\
struct ShaderSourceFiles {
	const char * const VertexShaderPath;
	const char * const FragmentShaderPath;

	ShaderSourceFiles(const char *vertexShaderPath, const char *fragmentShaderPath) noexcept
		: VertexShaderPath(vertexShaderPath),FragmentShaderPath(fragmentShaderPath) {
	}
};


enum class ShaderResource {
%s
};


class ShaderResources {
public:
	ShaderResources() = delete;
	static const ShaderSourceFiles& Get(ShaderResource identifier);

private:
	static ShaderSourceFiles shaderSourceFiles[];
};
"""


SHADER_RESOURCES_SOURCE_CPP_TEMPLATE = """\
#include "ShaderResources.h"

#include <type_traits>

ShaderSourceFiles ShaderResources::shaderSourceFiles[] = {
%s
};

const ShaderSourceFiles& ShaderResources::Get(ShaderResource identifier) {
	auto index = static_cast<std::underlying_type<ShaderResource>::type>(identifier);
	return shaderSourceFiles[index];
}
"""

SHADERS_RESOURCE_PATH = 'res/shaders/'
SHADER_RESOURCES_SOURCE_PATH = 'Engine/src/ResourceLoaders/'


logger = None


class ShaderType(Enum):
	VERTEX = 1
	FRAGMENT = 2


class ShaderSourceFiles:
	def __init__(self):
		self.vertex_shader_path = None
		self.fragment_shader_path = None

	def __str__(self):
		return f'VS Path: {self.vertex_shader_path}, FS Path: {self.fragment_shader_path}'


def setup_logger():
	logging.basicConfig(
		stream=sys.stdout,
		format="%(levelname)s %(asctime)s - %(message)s",
		level=logging.INFO
	)
	global logger
	logger = logging.getLogger()


def index_shaders() -> [(str, ShaderSourceFiles)]:
	shaders_table = dict()

	def add_shader_file(shader_name: str, shader_path: str, shader_type: ShaderType):
		if shader_name not in shaders_table:
			shaders_table[shader_name] = ShaderSourceFiles()

		if shader_type is ShaderType.VERTEX:
			shaders_table[shader_name].vertex_shader_path = shader_path
		elif shader_type is ShaderType.FRAGMENT:
			shaders_table[shader_name].fragment_shader_path = shader_path

	for filename in os.listdir(SHADERS_RESOURCE_PATH):
		file = os.path.join(SHADERS_RESOURCE_PATH, filename)
		if not os.path.isfile(file):
			continue

		if file.endswith('.vert.glsl'):
			add_shader_file(filename.removesuffix('.vert.glsl'), file, ShaderType.VERTEX)
		elif file.endswith('.frag.glsl'):
			add_shader_file(filename.removesuffix('.frag.glsl'), file, ShaderType.FRAGMENT)
		else:
			logging.warning(f'Found {file} file that doesn\'t conform with *.[vert|frag.glsl] format')

	return [(shader_name, shader_source_files) for (shader_name, shader_source_files) in shaders_table.items()]


def generate_shader_resources_h(resources: [(str, ShaderSourceFiles)]) -> str:
	shader_resources_enum_values = ''
	for shader_name, _ in resources:
		shader_resources_enum_values += f'\t{shader_name},\n'

	return SHADER_RESOURCES_SOURCE_H_TEMPLATE % shader_resources_enum_values


def generate_shader_resources_cpp(resources: [(str, ShaderSourceFiles)]) -> str:
	result = ''
	for (_, shader_files) in resources:
		result += f'\tShaderSourceFiles("{shader_files.vertex_shader_path}", "{shader_files.fragment_shader_path}"),\n'

	return SHADER_RESOURCES_SOURCE_CPP_TEMPLATE % result


def generate_source_files(resources: [(str, ShaderSourceFiles)]):
	with open(SHADER_RESOURCES_SOURCE_PATH + 'ShaderResources.h', 'w') as f:
		f.write(generate_shader_resources_h(resources))

	with open(SHADER_RESOURCES_SOURCE_PATH + 'ShaderResources.cpp', 'w') as f:
		f.write(generate_shader_resources_cpp(resources))


if __name__ == '__main__':
	logging.info(f'Launch script directory {os.getcwd()}')

	setup_logger()
	indexed_shaders = index_shaders()
	generate_source_files(indexed_shaders)
