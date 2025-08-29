#pragma once

// �����ڷ�
// https://computing-jhson.tistory.com/135

template<typename T>
class Singleton
{
private:
	Singleton() = default;

public:
	static T Get() 
	{
		static Singleton instance;
		return instance;
	}

private:
	// ����
	// ���硤�̵� ������
	T(const T& t) = delete;
	T(T&& t) = delete;
	// ���硤�̵� ����
	T& operator=(const T& t) = delete;
	T& operator=(T&& t) = delete;
};