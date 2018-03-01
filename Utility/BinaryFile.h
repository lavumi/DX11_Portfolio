#pragma once

class BinaryInputOutputHandler;
class BinaryWriter
{
public:
	BinaryWriter();
	~BinaryWriter();

	void Open(wstring filePath, UINT openOption = CREATE_ALWAYS);
	void Close();

	void Write(const bool data);
	void Write(const char data);
	void Write(const unsigned char data);
	void Write(const WORD data);
	void Write(const int data);
	void Write(const UINT data);
	void Write(const float data);
	void Write(const double data);
	void Write(const string& data);
	void Write(const wstring& data);

	void Write(const D3DXVECTOR2& data);
	void Write(const D3DXVECTOR3& data);
	void Write(const D3DXVECTOR4& data);
	void Write(const D3DXCOLOR& data);
	void Write(const D3DXMATRIX& data);
	void Write(const D3DXQUATERNION& data);

	void Write(const void* pointerAddress, UINT stride, UINT count);

	void Write(BinaryInputOutputHandler* data);

	template<typename T>
	void Write(const vector<T>& data);

	template<typename T1, typename T2>
	void Write(const map<T1, T2>& data);

protected:
	HANDLE fileHandle;
	DWORD size;
};

//////////////////////////////////////////////////////////////////////////

class BinaryReader
{
public:
	BinaryReader();
	~BinaryReader();

	void Open(wstring filePath);
	void Close();

	bool Bool();
	WORD Word();
	int Int();
	UINT UInt();
	float Float();
	double Double();
	string String();
	wstring WString();

	D3DXVECTOR2 Vector2();
	D3DXVECTOR3 Vector3();
	D3DXVECTOR4 Vector4();
	//D3DXCOLOR Color3f();
	D3DXCOLOR Color4f();
	D3DXMATRIX Matrix();
	D3DXQUATERNION Quaternion();

	void Read(void* pointerAddress, UINT stride, UINT count);

protected:
	HANDLE fileHandle;
	DWORD size;
};

template<typename T>
inline void BinaryWriter::Write(const vector<T>& data)
{
	UINT count = data.size();
	Write(count);
	for (size_t i = 0; i < count; i++)
	{
		Write(data[i]);
	}
}

template<typename T1, typename T2>
inline void BinaryWriter::Write(const map<T1, T2>& data)
{
	UINT count = data.size();
	Write(count);
	typename map<T1, T2>::const_iterator iter;
	for (iter = data.begin(); iter != data.end(); ++iter)
	{
		Write(iter->first);
		Write(iter->second);
	}
}
