#include <iostream>
#include "CharactersDraw.h"

//inspiration https://learnopengl.com/In-Practice/Text-Rendering

CharactersDraw::CharactersDraw(ShaderProgram* shdrPrg, float monitorWidth, float monitorHeight) : shaderProgram(shdrPrg) {
	geometry = new ObjectGeometry;

	

	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

	if (FT_New_Face(ft, "data/fonts/impact.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);
	mapAllCharacters();

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	projection = glm::ortho(0.0f, monitorWidth, 0.0f, monitorHeight);
	//glUniformMatrix4fv(shaderProgram->locations.projection, 1, GL_FALSE, glm::value_ptr(projection));

	geometry->elementBufferObject = 0;

	glGenVertexArrays(1, &geometry->vertexArrayObject);
	glBindVertexArray(geometry->vertexArrayObject);

	glGenBuffers(1, &geometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);
	//TODO mb static
	glBufferData(GL_ARRAY_BUFFER, 6*4*sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(shaderProgram->locations.position);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

CharactersDraw::~CharactersDraw() {
	glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
	glDeleteBuffers(1, &(geometry->elementBufferObject));
	glDeleteBuffers(1, &(geometry->vertexBufferObject));

	delete geometry;
	geometry = nullptr;
}

void CharactersDraw::update() {

}

void CharactersDraw::draw(std::string text, float posX, float posY, float scale, glm::vec3 color) {
	glUseProgram(shaderProgram->program);
	glUniform3f(shaderProgram->locations.textColor, color.x, color.y, color.z);

	glUniformMatrix4fv(shaderProgram->locations.projection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform1i(shaderProgram->locations.sampl, 3);

	glActiveTexture(GL_TEXTURE3);
	glBindVertexArray(geometry->vertexArrayObject);

	for (char i : text) {

		//std::cout << i;
		Character c = characters[i];
		/*std::cout << c.texture << " " << 
			characters[i].bearingX << " " <<
		characters[i].bearingY << " " <<
		characters[i].wigth << " " <<
		characters[i].height << " " <<
		characters[i].advance << std::endl;*/

		float x = posX + c.bearingX * scale;
		float y = posY - (c.height - c.bearingY) * scale;

		float width = c.wigth * scale;
		float height = c.height * scale;

		float vertices[6][4] = {
			{ x,         y + height,  0.0f, 0.0f },
			{ x,         y,			  0.0f, 1.0f },
			{ x + width, y,			  1.0f, 1.0f },

			{ x,         y + height,  0.0f, 0.0f },
			{ x + width, y,           1.0f, 1.0f },
			{ x + width, y + height,  1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, c.texture);
		glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//CHECK_GL_ERROR();
		posX += (c.advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CharactersDraw::mapAllCharacters() {
	for (unsigned char i = 0; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &characters[i].texture);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, characters[i].texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		characters[i].bearingX = face->glyph->bitmap_left;
		characters[i].bearingY = face->glyph->bitmap_top;
		characters[i].wigth = face->glyph->bitmap.width;
		characters[i].height = face->glyph->bitmap.rows;
		characters[i].advance = face->glyph->advance.x;

		//std::cout << *face->glyph->bitmap.buffer << std::endl;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}