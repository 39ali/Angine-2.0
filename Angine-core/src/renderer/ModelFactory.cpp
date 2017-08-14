#include "ModelFactory.h"
#include <math.h>

namespace Angine
{
	namespace Renderer {

#define M_PI  3.14159265359	
#define M_PI_2 M_PI/2
		Model* ModelFactory::createSphere(float radius, unsigned int rings, unsigned int sectors)
		{
			std::vector<VertexData> _data;
			std::vector<GLfloat> vertices;
			std::vector<GLfloat> normals;
			std::vector<GLfloat> texcoords;
			std::vector<unsigned int > indices;
			float const R = 1. / (float)(rings - 1);
			float const S = 1. / (float)(sectors - 1);
			int r, s;

			vertices.resize(rings * sectors * 3);
			normals.resize(rings * sectors * 3);
			texcoords.resize(rings * sectors * 2);
			std::vector<GLfloat>::iterator v = vertices.begin();
			std::vector<GLfloat>::iterator n = normals.begin();
			std::vector<GLfloat>::iterator t = texcoords.begin();
			for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
				float const y = sin(-M_PI_2 + M_PI * r * R);
				float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
				float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);
				VertexData vertex;
				vertex.uv.x = s*S;
				vertex.uv.y = r*R;
				/**t++ = s*S;
				*t++ = r*R;
*/
				vertex.position.x = x * radius;
				vertex.position.y = y * radius;
				vertex.position.z = z * radius;
				/*	*v++ = x * radius;
					*v++ = y * radius;
					*v++ = z * radius;*/

					/**n++ = x;
					*n++ = y;
					*n++ = z;*/

				vertex.normal.x = x;
				vertex.normal.y = y;
				vertex.normal.z = z;
				_data.push_back(vertex);
			}

			indices.resize(rings * sectors * 4);
			std::vector<unsigned int >::iterator i = indices.begin();
			for (r = 0; r < rings - 1; r++) for (s = 0; s < sectors - 1; s++) {
				*i++ = r * sectors + s;
				*i++ = r * sectors + (s + 1);
				*i++ = (r + 1) * sectors + (s + 1);
				*i++ = (r + 1) * sectors + s;
			}


			Model * m = new Model();
			GLuint sphereVAO = 0;
			GLuint vbo, ebo;
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);

			glGenVertexArrays(1, &sphereVAO);
			glBindVertexArray(sphereVAO);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, _data.size() * sizeof(float), _data.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void*)offsetof(VertexData, position));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void*)offsetof(VertexData, normal));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void*)offsetof(VertexData, uv));
			
			Mesh * mesh = new Mesh(sphereVAO, indices.size() / 4, ebo, GL_QUADS);
			m->m_meshes.push_back(mesh);
			return m;


		}


		Model* ModelFactory::createPlane()
		{
			float planeVertices[] = {
				// positions            // normals         // texcoords
				10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
				-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
				-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

				10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
				-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
				10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
			};
			// plane VAO
			unsigned int planeVAO, planeVBO;
			glGenVertexArrays(1, &planeVAO);
			glGenBuffers(1, &planeVBO);
			glBindVertexArray(planeVAO);
			glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glBindVertexArray(0);

			Model * m = new Model();
			Mesh * mesh = new Mesh(planeVAO,6, -1, GL_TRIANGLES);
			m->m_meshes.push_back(mesh);
			return m;

		}

	}
}