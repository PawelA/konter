#include <stdio.h>
#include <stdlib.h>

#include "gl.h"

int compile_shader(GLuint shader, const char *name) {
	FILE *fp;
	int size;
	char *buff;
	
	fp = fopen(name, "r");
	if (!fp) {
		printf("%s: no such file.\n", name);
		return 0;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	buff = (char *) malloc(size);
	if (!buff) {
		fclose(fp);
		return 0;
	}

	rewind(fp);
	fread(buff, size, 1, fp);
	fclose(fp);

	glShaderSource(shader, 1, (const char **) &buff, &size);
	glCompileShader(shader);
	free(buff);
	size = 0;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
	if (size > 0) {
		printf("%s: compilation error.\n", name);
		buff = (char *) malloc(size);
		if (buff) {
			glGetShaderInfoLog(shader, size, &size, buff);
			printf("%.*s", size, buff);
			free(buff);
		}
		return 0;
	}

	return 1;
}

int link_program(GLuint prog) {
	int size = 0;
	char *buff;

	glLinkProgram(prog);

	glGetShaderiv(prog, GL_INFO_LOG_LENGTH, &size);
	if (size > 0) {
		buff = (char *) malloc(size);
		if (buff) {
			glGetProgramInfoLog(prog, size, &size, buff);
			printf("%.*s", size, buff);
			free(buff);
		}
		return 0;
	}

	return 1;
}

