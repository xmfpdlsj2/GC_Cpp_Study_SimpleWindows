#pragma once
#include <exception>
#include <sstream>	

class ComException : public std::exception
{
private:
	HRESULT mResult;
	std::string description;

public:
	ComException(HRESULT hr, std::string msg) : mResult{ hr }, description{ msg } {};

	virtual const char* what() const override
	{
		//std::ostringstream oss;
		//oss << "Failure with HRESULT : " << mResult << std::endl;
		//return oss.str().c_str();

		static char buf[512]{};
		if (description.empty())
		{
			sprintf_s(buf, 512, "Failure with HRESULT : %X", mResult);
		}
		else
		{
			sprintf_s(buf, 512, "Failure with HRESULT : %X\n\n-> %s", mResult, description.c_str());
		}
		return buf;
	}
};

inline void ThrowIfFailed(HRESULT hr, std::string msg = "")
{
	if (FAILED(hr))
	{
		throw ComException(hr, msg);
	}
}