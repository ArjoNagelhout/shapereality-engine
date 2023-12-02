#include "transform.h"

namespace scene
{
	Transform::Transform() :
		localPosition({0.f, 0.f, 0.f}),
		localRotation(0.f, 0.f, 0.f, 1.f),
		localScale({1.f, 1.f, 1.f})
	{

	}

	Transform::~Transform() = default;

	int Transform::getChildCount()
	{
		return 0;
	}

	vec3 Transform::getEulerAngles()
	{
		return vec3{};
	}

	void Transform::setEulerAngles(vec3 const& eulerAngles)
	{

	}

	vec3 Transform::getForward()
	{
		return vec3{};
	}

	vec3 Transform::getLocalEulerAngles()
	{
		return vec3{};
	}

	void Transform::setLocalEulerAngles(vec3 const& localEulerAngles)
	{

	}

	vec3 Transform::getLocalPosition()
	{
		return vec3{};
	}

	void Transform::setLocalPosition(vec3 const& localPosition)
	{

	}

	Quaternion Transform::getLocalRotation()
	{
		return Quaternion(0.f, 0.f, 0.f, 1.f);
	}

	void Transform::setLocalRotation(Quaternion const& localRotation)
	{

	}

	vec3 Transform::getLocalScale()
	{
		return vec3{};
	}

	void Transform::setLocalScale(vec3 const& localScale)
	{

	}

	mat4 Transform::getLocalToWorldMatrix()
	{
		return mat4();
	}

	vec3 Transform::getLossyScale()
	{
		return vec3{};
	}

	Transform* Transform::getParent()
	{
		return nullptr;
	}

	vec3 Transform::getPosition()
	{
		return vec3{};
	}

	void Transform::setPosition(vec3 const& position)
	{

	}

	vec3 Transform::getRight()
	{
		return vec3{};
	}

	Transform* Transform::getRoot()
	{
		return nullptr;
	}

	Quaternion Transform::getRotation()
	{
		return Quaternion(0.f, 0.f, 0.f, 1.f);
	}

	void Transform::setRotation(Quaternion const& rotation)
	{

	}

	vec3 Transform::getUp()
	{
		return vec3{};
	}

	mat4 Transform::getWorldToLocalMatrix()
	{
		return mat4();
	}

	// public methods

	void Transform::detachChildren()
	{

	}

	Transform* Transform::find(std::string const& objectName)
	{
		return nullptr;
	}

	Transform* Transform::getChild(int const& index)
	{
		return nullptr;
	}

	void Transform::getLocalPositionAndRotation(vec3& localPosition, Quaternion& localRotation)
	{

	}

	void Transform::getPositionAndRotation(vec3& position, vec3& rotation)
	{

	}

	int Transform::getSiblingIndex()
	{
		return 0;
	}

	vec3 Transform::inverseTransformDirection(vec3 const& direction)
	{
		return vec3{};
	}

	vec3 Transform::inverseTransformDirection(float const& x, float const& y, float const& z)
	{
		return vec3{};
	}

	void Transform::inverseTransformDirections(std::vector<vec3>& directions)
	{

	}

	vec3 Transform::inverseTransformPoint(vec3 const& position)
	{
		return vec3{};
	}

	void Transform::inverseTransformPoints(std::vector<vec3>& positions)
	{

	}

	vec3 Transform::inverseTransformVector(vec3 const& vector)
	{
		return vec3{};
	}

	void Transform::inverseTransformVectors(std::vector<vec3>& vectors)
	{

	}

	[[nodiscard]] bool Transform::isChildOf(Transform* const parent)
	{
		return false;
	}

	void Transform::lookAt(Transform* const target, vec3 const& worldUp)
	{

	}

	void Transform::rotate(vec3 const& eulers, Space relativeTo)
	{

	}

	void Transform::rotate(float const& xAngle, float const& yAngle, float const& zAngle, Space relativeTo)
	{

	}

	void Transform::rotate(vec3 const& axis, float const& angle, Space relativeTo)
	{

	}

	void Transform::rotateAround(vec3 const& point, vec3 const& axis, float const& angle)
	{

	}

	void Transform::setAsFirstSibling()
	{

	}

	void Transform::setAsLastSibling()
	{

	}

	void Transform::setLocalPositionAndRotation(vec3 const& localPosition, Quaternion const& localRotation)
	{

	}

	void Transform::setParent(Transform* parent, bool worldPositionStays)
	{

	}

	void Transform::setPositionAndRotation(vec3 const& position, Quaternion const& rotation)
	{

	}

	void Transform::setSiblingIndex(int const& index)
	{

	}

	vec3 Transform::transformDirection(vec3 const& direction)
	{
		return vec3{};
	}

	vec3 Transform::transformDirection(float const& x, float const& y, float const& z)
	{
		return vec3{};
	}

	void Transform::transformDirections(std::vector<vec3>& directions)
	{

	}

	vec3 Transform::transformPoint(vec3 const& position)
	{
		return vec3{};
	}

	void Transform::transformPoints(std::vector<vec3>& positions)
	{

	}

	vec3 Transform::transformVector(vec3 const& vector)
	{
		return vec3{};
	}

	void Transform::transformVectors(std::vector<vec3>& vectors)
	{

	}

	void Transform::translate(vec3 const& translation, Space relativeTo)
	{

	}
}