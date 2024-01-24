#pragma once
#include <string>



struct IService
{
	virtual std::string GetServiceName() = 0;
};

#define IMPL_ISERVICE(serviceName) public:\
static std::string GetServiceNameStatic() { return #serviceName; }\
\
std::string GetServiceName() override { return GetServiceNameStatic(); }

template <typename T>
concept IServiceDerived = requires {
	{ T::GetServiceNameStatic() } -> std::same_as<std::string>;
} && std::derived_from<T, IService>;