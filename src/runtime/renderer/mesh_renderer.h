//
// Created by Arjo Nagelhout on 27/12/2023.
//

#ifndef SHAPEREALITY_MESH_RENDERER_H
#define SHAPEREALITY_MESH_RENDERER_H

namespace renderer
{
	class Mesh;
	class Material;

	class MeshRenderer final
	{
	public:
		explicit MeshRenderer(Mesh* pMesh, Material* pMaterial);
		~MeshRenderer();

	private:
		Mesh* pMesh; // unowned pointer
		Material *pMaterial; // unowned pointer
	};
}

#endif //SHAPEREALITY_MESH_RENDERER_H
