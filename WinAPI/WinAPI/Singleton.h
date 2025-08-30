#pragma once

// 참고자료
// https://computing-jhson.tistory.com/135

template<typename T>
class Singleton
{
protected:
	Singleton() = default;
	~Singleton() = default;

protected:
	static T& Get() 
	{
		static T instance;
		return instance;
	}

protected:
	// 방지
	// Singleton만 봉쇄하고, 다른 타입(T)은 자유롭게 생성 및 대입
	// 
	// 복사·이동 생성자
	Singleton(const Singleton& t) = delete;
	Singleton(Singleton&& t) = delete;
	// 복사·이동 대입
	Singleton& operator=(const Singleton& t) = delete;
	Singleton& operator=(Singleton&& t) = delete;
};