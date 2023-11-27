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

	Vector3 Transform::getEulerAngles()
	{
		return Vector3{};
	}

	void Transform::setEulerAngles(Vector3 const& eulerAngles)
	{

	}

	Vector3 Transform::getForward()
	{
		return Vector3{};
	}

	Vector3 Transform::getLocalEulerAngles()
	{
		return Vector3{};
	}

	void Transform::setLocalEulerAngles(Vector3 const& localEulerAngles)
	{

	}

	Vector3 Transform::getLocalPosition()
	{
		return Vector3{};
	}

	void Transform::setLocalPosition(Vector3 const& localPosition)
	{

	}

	Quaternion Transform::getLocalRotation()
	{
		return Quaternion(0.f, 0.f, 0.f, 1.f);
	}

	void Transform::setLocalRotation(Quaternion const& localRotation)
	{

	}

	Vector3 Transform::getLocalScale()
	{
		return Vector3{};
	}

	void Transform::setLocalScale(Vector3 const& localScale)
	{

	}

	Matrix4x4 Transform::getLocalToWorldMatrix()
	{
		return Matrix4x4();
	}

	Vector3 Transform::getLossyScale()
	{
		return Vector3{};
	}

	Transform* Transform::getParent()
	{
		return nullptr;
	}

	Vector3 Transform::getPosition()
	{
		return Vector3{};
	}

	void Transform::setPosition(Vector3 const& position)
	{

	}

	Vector3 Transform::getRight()
	{
		return Vector3{};
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

	Vector3 Transform::getUp()
	{
		return Vector3{};
	}

	Matrix4x4 Transform::getWorldToLocalMatrix()
	{
		return Matrix4x4();
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

	void Transform::getLocalPositionAndRotation(Vector3& localPosition, Quaternion& localRotation)
	{

	}

	void Transform::getPositionAndRotation(Vector3& position, Vector3& rotation)
	{

	}

	int Transform::getSiblingIndex()
	{
		return 0;
	}

	Vector3 Transform::inverseTransformDirection(Vector3 const& direction)
	{
		return Vector3{};
	}

	Vector3 Transform::inverseTransformDirection(float const& x, float const& y, float const& z)
	{
		return Vector3{};
	}

	void Transform::inverseTransformDirections(std::vector<Vector3>& directions)
	{

	}

	Vector3 Transform::inverseTransformPoint(Vector3 const& position)
	{
		return Vector3{};
	}

	void Transform::inverseTransformPoints(std::vector<Vector3>& positions)
	{

	}

	Vector3 Transform::inverseTransformVector(Vector3 const& vector)
	{
		return Vector3{};
	}

	void Transform::inverseTransformVectors(std::vector<Vector3>& vectors)
	{

	}

	[[nodiscard]] bool Transform::isChildOf(Transform* const parent)
	{
		return false;
	}

	void Transform::lookAt(Transform* const target, Vector3 const& worldUp)
	{

	}

	void Transform::rotate(Vector3 const& eulers, Space relativeTo)
	{

	}

	void Transform::rotate(float const& xAngle, float const& yAngle, float const& zAngle, Space relativeTo)
	{

	}

	void Transform::rotate(Vector3 const& axis, float const& angle, Space relativeTo)
	{

	}

	void Transform::rotateAround(Vector3 const& point, Vector3 const& axis, float const& angle)
	{

	}

	void Transform::setAsFirstSibling()
	{

	}

	void Transform::setAsLastSibling()
	{

	}

	void Transform::setLocalPositionAndRotation(Vector3 const& localPosition, Quaternion const& localRotation)
	{

	}

	void Transform::setParent(Transform* parent, bool worldPositionStays)
	{

	}

	void Transform::setPositionAndRotation(Vector3 const& position, Quaternion const& rotation)
	{

	}

	void Transform::setSiblingIndex(int const& index)
	{

	}

	Vector3 Transform::transformDirection(Vector3 const& direction)
	{
		return Vector3{};
	}

	Vector3 Transform::transformDirection(float const& x, float const& y, float const& z)
	{
		return Vector3{};
	}

	void Transform::transformDirections(std::vector<Vector3>& directions)
	{

	}

	Vector3 Transform::transformPoint(Vector3 const& position)
	{
		return Vector3{};
	}

	void Transform::transformPoints(std::vector<Vector3>& positions)
	{

	}

	Vector3 Transform::transformVector(Vector3 const& vector)
	{
		return Vector3{};
	}

	void Transform::transformVectors(std::vector<Vector3>& vectors)
	{

	}

	void Transform::translate(Vector3 const& translation, Space relativeTo)
	{

	}
}