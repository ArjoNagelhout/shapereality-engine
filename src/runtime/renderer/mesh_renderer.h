//
// Created by Arjo Nagelhout on 27/12/2023.
//

#ifndef SHAPEREALITY_MESH_RENDERER_H
#define SHAPEREALITY_MESH_RENDERER_H

#include "math/matrix.h"
#include "math/vector.h"
#include "math/quaternion.h"

namespace renderer
{
	class Mesh;

	class Material;

	class MeshRenderer final
	{
	public:
		explicit MeshRenderer(Mesh* pMesh, Material* pMaterial);

		~MeshRenderer();

		//
		[[nodiscard]] math::mat4 getTransform();

		//
		[[nodiscard]] math::vec3 getLocalPosition() const;

		//
		void setLocalPosition(math::vec3 _localPosition);

		//
		[[nodiscard]] math::Quaternion getLocalRotation() const;

		//
		void setLocalRotation(math::Quaternion _localRotation);

		//
		[[nodiscard]] math::vec3 getLocalScale() const;

		//
		void setLocalScale(math::vec3 _localScale);

		//
		[[nodiscard]] Mesh* getMesh() const;

		//
		[[nodiscard]] Material* getMaterial() const;

	private:
		// todo refactor transform into Transform class, look into ECS.
		math::vec3 localPosition{math::vec3::zero};
		math::Quaternion localRotation{math::Quaternion::identity};
		math::vec3 localScale{math::vec3::one};

		math::mat4 transform;
		bool transformIsDirty = true;

		void updateTransform();

		Mesh* pMesh; // unowned pointer
		Material* pMaterial; // unowned pointer
	};
}

#endif //SHAPEREALITY_MESH_RENDERER_H
