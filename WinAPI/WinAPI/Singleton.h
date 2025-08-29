#pragma once

// 참고자료
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
	// 방지
	// 복사·이동 생성자
	T(const T& t) = delete;
	T(T&& t) = delete;
	// 복사·이동 대입
	T& operator=(const T& t) = delete;
	T& operator=(T&& t) = delete;
};