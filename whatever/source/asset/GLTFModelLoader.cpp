#include "GLTFModelLoader.h"
#define TINYGLTF_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tinygltf/tiny_gltf.h"

namespace wtv
{
	VertexAttributeType GetVertexAttribTypeFromTinyGLTF(int componentType, int type);
	VertexAttributeSemantic GetVertexAttribSemanticFromTinyGLTF(const std::string semantic);
	IndexType GetIndexTypeFromTinyGLTF(int componentType);

	std::vector<CPUMesh> GLTFModelLoader::LoadModel(const char* filePath)
	{
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;

		std::string err;
		std::string warn;

		bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filePath);

		std::vector<CPUMesh> meshes;
		meshes.reserve(model.meshes.size());

		for (int i = 0; i < model.meshes.size(); i++)
		{
			const tinygltf::Mesh& gltfMesh = model.meshes[i];
			for (auto& prim : gltfMesh.primitives)
			{
				IndexType indexType = GetIndexTypeFromTinyGLTF(model.accessors[prim.indices].componentType);
				std::vector<VertexAttribute> vertexAttribs(prim.attributes.size());
				int attribIndex = 0;
				for (auto& attrib : prim.attributes)
				{
					auto accessorIndex = attrib.second;
					auto& accessor = model.accessors[accessorIndex];
					vertexAttribs[attribIndex].type = GetVertexAttribTypeFromTinyGLTF(accessor.componentType, accessor.type);
					vertexAttribs[attribIndex].semantic = GetVertexAttribSemanticFromTinyGLTF(attrib.first);
					attribIndex++;
				}
				
				auto& indicesAccessor = model.accessors[prim.indices];
				MeshInfo meshInfo(vertexAttribs.data(), vertexAttribs.size(), indexType);
				meshes.emplace_back(meshInfo);

				// Upload indices

				const tinygltf::BufferView& indexBufferView = model.bufferViews[indicesAccessor.bufferView];
				const tinygltf::Buffer& indexBuffer = model.buffers[indexBufferView.buffer];
				uint32_t indexSize = tinygltf::GetComponentSizeInBytes(indicesAccessor.componentType);
				uint32_t dataSize = indicesAccessor.count * indexSize;

				meshes.back().SetIndices(
					indexBuffer.data.data() + indexBufferView.byteOffset + indicesAccessor.byteOffset,
					dataSize
				);
				

				uint32_t vertexCount = model.accessors[prim.attributes.at("POSITION")].count;
				meshes.back().SetVertexCount(vertexCount);
				// Upload all vertex attribute streams
				for (auto& attrib : prim.attributes)
				{
					const tinygltf::Accessor& accessor = model.accessors[attrib.second];
					const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
					const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
					uint32_t elementSize = tinygltf::GetNumComponentsInType(accessor.type)
						* tinygltf::GetComponentSizeInBytes(accessor.componentType);

					VertexAttributeSemantic semantic = GetVertexAttribSemanticFromTinyGLTF(attrib.first);

					if (semantic != VertexAttributeSemantic::Position)
					{
						meshes.back().SetVerticesAttribute(
							semantic,
							buffer.data.data() + bufferView.byteOffset,
							bufferView.byteLength,
							bufferView.byteStride,
							elementSize,
							vertexCount

						);
					}
					else
					{
						auto convertPosition = [elementSize](void* vertexData) {
							if (elementSize == 12)
							{
								float* pos = static_cast<float*>(vertexData);
								//std::swap(pos[0], pos[1]);
								std::swap(pos[1], pos[2]);
								pos[1] *= -1;
							}
							else if (elementSize == 24)
							{
								double* pos = static_cast<double*>(vertexData);
								//std::swap(pos[0], pos[1]);
								std::swap(pos[1], pos[2]);
							}
						};
						meshes.back().SetVerticesAttribute(
							semantic,
							buffer.data.data() + bufferView.byteOffset,
							bufferView.byteLength,
							bufferView.byteStride,
							elementSize,
							vertexCount,
							convertPosition
						);
					}
				}
				meshes.back().FormVertexBuffer();
			}
		}
			
		return meshes;
	}

	VertexAttributeType GetVertexAttribTypeFromTinyGLTF(int componentType, int type)
	{
		switch (type)
		{
			case TINYGLTF_TYPE_SCALAR:
			switch (componentType)
			{
			case TINYGLTF_COMPONENT_TYPE_BYTE:
				return VertexAttributeType::byte1;
			case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
				return VertexAttributeType::ubyte1;
			case TINYGLTF_COMPONENT_TYPE_SHORT:
				return VertexAttributeType::short1;
			case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
				return VertexAttributeType::ushort1;
			case TINYGLTF_COMPONENT_TYPE_INT:
				return VertexAttributeType::int1;
			case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
				return VertexAttributeType::uint1;
			case TINYGLTF_COMPONENT_TYPE_FLOAT:
				return VertexAttributeType::float1;
			default:
				assert(false);
				return VertexAttributeType::Undefined;
			}
			case TINYGLTF_TYPE_VEC2:
				switch (componentType)
				{
				case TINYGLTF_COMPONENT_TYPE_BYTE:
					return VertexAttributeType::byte2;
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
					return VertexAttributeType::ubyte2;
				case TINYGLTF_COMPONENT_TYPE_SHORT:
					return VertexAttributeType::short2;
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
					return VertexAttributeType::ushort2;
				case TINYGLTF_COMPONENT_TYPE_INT:
					return VertexAttributeType::int2;
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
					return VertexAttributeType::uint2;
				case TINYGLTF_COMPONENT_TYPE_FLOAT:
					return VertexAttributeType::float2;
				default:
					assert(false);
					return VertexAttributeType::Undefined;
				}
			case TINYGLTF_TYPE_VEC3:
				switch (componentType)
				{
				case TINYGLTF_COMPONENT_TYPE_BYTE:
					return VertexAttributeType::byte3;
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
					return VertexAttributeType::ubyte3;
				case TINYGLTF_COMPONENT_TYPE_SHORT:
					return VertexAttributeType::short3;
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
					return VertexAttributeType::ushort3;
				case TINYGLTF_COMPONENT_TYPE_INT:
					return VertexAttributeType::int3;
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
					return VertexAttributeType::uint3;
				case TINYGLTF_COMPONENT_TYPE_FLOAT:
					return VertexAttributeType::float3;
				default:
					assert(false);
					return VertexAttributeType::Undefined;
				}
			case TINYGLTF_TYPE_VEC4:
				switch (componentType)
				{
				case TINYGLTF_COMPONENT_TYPE_BYTE:
					return VertexAttributeType::byte4;
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
					return VertexAttributeType::ubyte4;
				case TINYGLTF_COMPONENT_TYPE_SHORT:
					return VertexAttributeType::short4;
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
					return VertexAttributeType::ushort4;
				case TINYGLTF_COMPONENT_TYPE_INT:
					return VertexAttributeType::int4;
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
					return VertexAttributeType::uint4;
				case TINYGLTF_COMPONENT_TYPE_FLOAT:
					return VertexAttributeType::float4;
				default:
					assert(false);
					return VertexAttributeType::Undefined;
				}
			default:
				assert(false);
				return VertexAttributeType::Undefined;
		}
	}

	VertexAttributeSemantic GetVertexAttribSemanticFromTinyGLTF(const std::string semantic)
	{
		if (semantic == "POSITION")
			return VertexAttributeSemantic::Position;
		else if(semantic == "NORMAL")
			return VertexAttributeSemantic::Normal;
		else if(semantic == "TANGENT")
			return VertexAttributeSemantic::Tangent;
		else if(semantic == "COLOR")
			return VertexAttributeSemantic::Color;
		else if(semantic == "TEXCOORD_0")
			return VertexAttributeSemantic::TexCoord0;
		else if (semantic == "TEXCOORD_1")
			return VertexAttributeSemantic::TexCoord1;
		else if(semantic == "TEXCOORD_2")
			return VertexAttributeSemantic::TexCoord2;
		else if(semantic == "TEXCOORD_3")
			return VertexAttributeSemantic::TexCoord3;
		else if(semantic == "TEXCOORD_4")
			return VertexAttributeSemantic::TexCoord4;
		else
		{
			assert(false);
			return VertexAttributeSemantic::Undefined;
		}
	}

	IndexType GetIndexTypeFromTinyGLTF(int componentType)
	{
		switch (componentType)
		{
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
			return IndexType::UInt16;
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
			return IndexType::UInt32;
		default:
			assert(false);
			return IndexType::Undefined;
		}
	}

}
