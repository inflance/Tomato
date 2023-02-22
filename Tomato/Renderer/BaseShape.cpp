#include "BaseShape.h"


namespace Tomato {

	void Tomato::BaseShape::CreateCube(int entityID)
	{

		m_base_shape.Vertices.resize(36);
		m_base_shape.Vertices[0].Position =  {   -0.5f, -0.5f, -0.5f,};
		m_base_shape.Vertices[1].Position =  {    0.5f, -0.5f, -0.5f,};
		m_base_shape.Vertices[2].Position =  {    0.5f,  0.5f, -0.5f,};
		m_base_shape.Vertices[3].Position =  {    0.5f,  0.5f, -0.5f,};
		m_base_shape.Vertices[4].Position =  {   -0.5f,  0.5f, -0.5f,};
		m_base_shape.Vertices[5].Position =  {   -0.5f, -0.5f, -0.5f, };

		m_base_shape.Vertices[6].Position =  {  -0.5f, -0.5f,  0.5f,};
		m_base_shape.Vertices[7].Position =  {   0.5f, -0.5f,  0.5f,};
		m_base_shape.Vertices[8].Position =  {   0.5f,  0.5f,  0.5f,};
		m_base_shape.Vertices[9].Position =  {   0.5f,  0.5f,  0.5f,};
		m_base_shape.Vertices[10].Position = {  -0.5f,  0.5f,  0.5f,};
		m_base_shape.Vertices[11].Position = {  -0.5f, -0.5f,  0.5f,};

		m_base_shape.Vertices[12].Position = { -0.5f,  0.5f,  0.5f,};
		m_base_shape.Vertices[13].Position = { -0.5f,  0.5f, -0.5f,};
		m_base_shape.Vertices[14].Position = { -0.5f, -0.5f, -0.5f,};
		m_base_shape.Vertices[15].Position = { -0.5f, -0.5f, -0.5f,};
		m_base_shape.Vertices[16].Position = { -0.5f, -0.5f,  0.5f,};
		m_base_shape.Vertices[17].Position = { -0.5f,  0.5f,  0.5f,};

		m_base_shape.Vertices[18].Position = {  0.5f,  0.5f,  0.5f, };
		m_base_shape.Vertices[19].Position = {  0.5f,  0.5f, -0.5f, };
		m_base_shape.Vertices[20].Position = {  0.5f, -0.5f, -0.5f, };
		m_base_shape.Vertices[21].Position = {  0.5f, -0.5f, -0.5f, };
		m_base_shape.Vertices[22].Position = {  0.5f, -0.5f,  0.5f, };
		m_base_shape.Vertices[23].Position = {  0.5f,  0.5f,  0.5f, };

		m_base_shape.Vertices[24].Position = { -0.5f, -0.5f, -0.5f,};
		m_base_shape.Vertices[25].Position = {  0.5f, -0.5f, -0.5f,};
		m_base_shape.Vertices[26].Position = {  0.5f, -0.5f,  0.5f,};
		m_base_shape.Vertices[27].Position = {  0.5f, -0.5f,  0.5f,};
		m_base_shape.Vertices[28].Position = { -0.5f, -0.5f,  0.5f,};
		m_base_shape.Vertices[29].Position = { -0.5f, -0.5f, -0.5f,};

		m_base_shape.Vertices[30].Position = {-0.5f,  0.5f, -0.5f, };
		m_base_shape.Vertices[31].Position = { 0.5f,  0.5f, -0.5f, };
		m_base_shape.Vertices[32].Position = { 0.5f,  0.5f,  0.5f, };
		m_base_shape.Vertices[33].Position = { 0.5f,  0.5f,  0.5f, };
		m_base_shape.Vertices[34].Position = {-0.5f,  0.5f,  0.5f, };
		m_base_shape.Vertices[35].Position = {-0.5f,  0.5f, -0.5f, };

		m_base_shape.Vertices[0].Normal = { 0.0f,  0.0f, -1.0f, };
		m_base_shape.Vertices[1].Normal = { 0.0f,  0.0f, -1.0f, };
		m_base_shape.Vertices[2].Normal = { 0.0f,  0.0f, -1.0f, };
		m_base_shape.Vertices[3].Normal = { 0.0f,  0.0f, -1.0f, };
		m_base_shape.Vertices[4].Normal = { 0.0f,  0.0f, -1.0f, };
		m_base_shape.Vertices[5].Normal = { 0.0f,  0.0f, -1.0f, };

		m_base_shape.Vertices[6].Normal = { 0.0f,  0.0f,  1.0f, };
		m_base_shape.Vertices[7].Normal = { 0.0f,  0.0f,  1.0f, };
		m_base_shape.Vertices[8].Normal = { 0.0f,  0.0f,  1.0f, };
		m_base_shape.Vertices[9].Normal = { 0.0f,  0.0f,  1.0f, };
		m_base_shape.Vertices[10].Normal = { 0.0f,  0.0f,  1.0f, };
		m_base_shape.Vertices[11].Normal = { 0.0f,  0.0f,  1.0f, };

		m_base_shape.Vertices[12].Normal = { -1.0f,  0.0f,  0.0f, };
		m_base_shape.Vertices[13].Normal = { -1.0f,  0.0f,  0.0f, };
		m_base_shape.Vertices[14].Normal = { -1.0f,  0.0f,  0.0f, };
		m_base_shape.Vertices[15].Normal = { -1.0f,  0.0f,  0.0f, };
		m_base_shape.Vertices[16].Normal = { -1.0f,  0.0f,  0.0f, };
		m_base_shape.Vertices[17].Normal = { -1.0f,  0.0f,  0.0f, };

		m_base_shape.Vertices[18].Normal = {1.0f,  0.0f,  0.0f, };
		m_base_shape.Vertices[19].Normal = {1.0f,  0.0f,  0.0f, };
		m_base_shape.Vertices[20].Normal = {1.0f,  0.0f,  0.0f, };
		m_base_shape.Vertices[21].Normal = {1.0f,  0.0f,  0.0f, };
		m_base_shape.Vertices[22].Normal = {1.0f,  0.0f,  0.0f, };
		m_base_shape.Vertices[23].Normal = {1.0f,  0.0f,  0.0f, };

		m_base_shape.Vertices[24].Normal = { 0.0f, -1.0f,  0.0f, };
		m_base_shape.Vertices[25].Normal = { 0.0f, -1.0f,  0.0f, };
		m_base_shape.Vertices[26].Normal = { 0.0f, -1.0f,  0.0f, };
		m_base_shape.Vertices[27].Normal = { 0.0f, -1.0f,  0.0f, };
		m_base_shape.Vertices[28].Normal = { 0.0f, -1.0f,  0.0f, };
		m_base_shape.Vertices[29].Normal = { 0.0f, -1.0f,  0.0f, };

		m_base_shape.Vertices[30].Normal = { 0.0f,  1.0f,  0.0f, };
		m_base_shape.Vertices[31].Normal = { 0.0f,  1.0f,  0.0f, };
		m_base_shape.Vertices[32].Normal = { 0.0f,  1.0f,  0.0f, };
		m_base_shape.Vertices[33].Normal = { 0.0f,  1.0f,  0.0f, };
		m_base_shape.Vertices[34].Normal = { 0.0f,  1.0f,  0.0f, };
		m_base_shape.Vertices[35].Normal = { 0.0f,  1.0f,  0.0f  };

		m_base_shape.Vertices[0]. TexCoords = { 0.0f, 0.0f };
		m_base_shape.Vertices[1]. TexCoords = { 1.0f, 0.0f,};
		m_base_shape.Vertices[2]. TexCoords = { 1.0f, 1.0f };
		m_base_shape.Vertices[3]. TexCoords = { 1.0f, 1.0f };
		m_base_shape.Vertices[4]. TexCoords = { 0.0f, 1.0f };
		m_base_shape.Vertices[5]. TexCoords = { 0.0f, 0.0f };

		m_base_shape.Vertices[6]. TexCoords = { 0.0f, 0.0f };
		m_base_shape.Vertices[7]. TexCoords = { 1.0f, 0.0f,};
		m_base_shape.Vertices[8]. TexCoords = { 1.0f, 1.0f };
		m_base_shape.Vertices[9]. TexCoords = { 1.0f, 1.0f };
		m_base_shape.Vertices[10].TexCoords = { 0.0f, 1.0f };
		m_base_shape.Vertices[11].TexCoords = { 0.0f, 0.0f };

		m_base_shape.Vertices[12].TexCoords = { 1.0f, 0.0f };
		m_base_shape.Vertices[13].TexCoords = { 1.0f, 1.0f };
		m_base_shape.Vertices[14].TexCoords = { 0.0f, 1.0f };
		m_base_shape.Vertices[15].TexCoords = { 0.0f, 1.0f };
		m_base_shape.Vertices[16].TexCoords = { 0.0f, 0.0f };
		m_base_shape.Vertices[17].TexCoords = { 1.0f, 0.0f };

		m_base_shape.Vertices[18].TexCoords = { 1.0f, 0.0f };
		m_base_shape.Vertices[19].TexCoords = { 1.0f, 1.0f,};
		m_base_shape.Vertices[20].TexCoords = { 0.0f, 1.0f };
		m_base_shape.Vertices[21].TexCoords = { 0.0f, 1.0f };
		m_base_shape.Vertices[22].TexCoords = { 0.0f, 0.0f };
		m_base_shape.Vertices[23].TexCoords = { 1.0f, 0.0f };

		m_base_shape.Vertices[24].TexCoords = { 0.0f, 1.0f } ;
		m_base_shape.Vertices[25].TexCoords = { 1.0f, 1.0f,} ;
		m_base_shape.Vertices[26].TexCoords = { 1.0f, 0.0f } ;
		m_base_shape.Vertices[27].TexCoords = { 1.0f, 0.0f } ;
		m_base_shape.Vertices[28].TexCoords = { 0.0f, 0.0f } ;
		m_base_shape.Vertices[29].TexCoords = { 0.0f, 1.0f } ;

		m_base_shape.Vertices[30].TexCoords = { 0.0f, 1.0f };
		m_base_shape.Vertices[31].TexCoords = { 1.0f, 1.0f,};
		m_base_shape.Vertices[32].TexCoords = { 1.0f, 0.0f };
		m_base_shape.Vertices[33].TexCoords = { 1.0f, 0.0f };
		m_base_shape.Vertices[34].TexCoords = { 0.0f, 0.0f };
		m_base_shape.Vertices[35].TexCoords = { 0.0f, 1.0f }; 
							
		m_base_shape.Vertices[0].EntityID = entityID;
		m_base_shape.Vertices[1].EntityID = entityID;
		m_base_shape.Vertices[2].EntityID = entityID;
		m_base_shape.Vertices[3].EntityID = entityID;
		m_base_shape.Vertices[4].EntityID = entityID;
		m_base_shape.Vertices[5].EntityID = entityID;

		m_base_shape.Vertices[6].EntityID = entityID;
		m_base_shape.Vertices[7].EntityID = entityID;
		m_base_shape.Vertices[8].EntityID = entityID;
		m_base_shape.Vertices[9].EntityID = entityID;
		m_base_shape.Vertices[10].EntityID = entityID;
		m_base_shape.Vertices[11].EntityID = entityID;

		m_base_shape.Vertices[12].EntityID = entityID;
		m_base_shape.Vertices[13].EntityID = entityID;
		m_base_shape.Vertices[14].EntityID = entityID;
		m_base_shape.Vertices[15].EntityID = entityID;
		m_base_shape.Vertices[16].EntityID = entityID;
		m_base_shape.Vertices[17].EntityID = entityID;

		m_base_shape.Vertices[18].EntityID = entityID;
		m_base_shape.Vertices[19].EntityID = entityID;
		m_base_shape.Vertices[20].EntityID = entityID;
		m_base_shape.Vertices[21].EntityID = entityID;
		m_base_shape.Vertices[22].EntityID = entityID;
		m_base_shape.Vertices[23].EntityID = entityID;

		m_base_shape.Vertices[24].EntityID = entityID;
		m_base_shape.Vertices[25].EntityID = entityID;
		m_base_shape.Vertices[26].EntityID = entityID;
		m_base_shape.Vertices[27].EntityID = entityID;
		m_base_shape.Vertices[28].EntityID = entityID;
		m_base_shape.Vertices[29].EntityID = entityID;

		m_base_shape.Vertices[30].EntityID = entityID;;
		m_base_shape.Vertices[31].EntityID = entityID;;
		m_base_shape.Vertices[32].EntityID = entityID;;
		m_base_shape.Vertices[33].EntityID = entityID;;
		m_base_shape.Vertices[34].EntityID = entityID;;
		m_base_shape.Vertices[35].EntityID = entityID;


		//m_base_shape.indices.resize(0);
		/*m_base_shape.indices = {
			0, 1, 2,
			2, 3, 0,
			1, 5, 6,
			6, 2, 1,
			7, 6, 5,
			5, 4, 7,
			4, 0, 3,
			3, 7, 4,
			4, 5, 1,
			1, 0, 4,
			3, 2, 6,
			6, 7, 3,
		};*/

		m_vertex_array = VertexArray::Create();
		
		m_vertex_buffer = VertexBuffer::Create(sizeof(Vertex) * m_base_shape.Vertices.size());
		m_vertex_buffer->SetLayout({
					{ ShaderDataType::Float3, "a_Position"},
					{ ShaderDataType::Float3, "a_Normal"},
					{ ShaderDataType::Float2, "a_TexCoord"},
					{ ShaderDataType::Float3, "a_Tangent"},
					{ ShaderDataType::Float3, "a_Bitangent"},
					{ ShaderDataType::Int,	  "a_EntityID"},
		});
		m_vertex_array->AddVertexBuffer(m_vertex_buffer);

		/*m_index_buffer = IndexBuffer::Create(m_base_shape.indices.data(), m_base_shape.indices.size());
		m_vertex_array->SetIndexBuffer(m_index_buffer);*/
	}


	BaseShape::BaseShape(int entityID)
	{
		CreateCube(entityID);
	}

	void BaseShape::SetData()
	{
		m_vertex_buffer->SetData(m_base_shape.Vertices.data(), m_base_shape.Vertices.size() * sizeof(Vertex));
	}

}

