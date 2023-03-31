#pragma once
#ifndef DEFINES_H
#define DEFINES_H
#include<iostream>
struct person
{
	char id[19];
	char name[20];
	char address[60];
	char phone_num[14];
	
	person() = default;
	person(const person&) = default;
	person& operator=(const person&) = default;
	person(person&&)noexcept = default;
	person& operator=(person&&)noexcept=default;
	~person() = default;
};
#endif // !DEFINES_H
