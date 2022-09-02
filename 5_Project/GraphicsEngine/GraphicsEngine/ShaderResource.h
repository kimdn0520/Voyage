#pragma once

class ConstantBuffer
{
public:
	ConstantBuffer(string paName, int slot_num, int paSize)
		:	name(paName), register_slot(slot_num), size(paSize){}
	
public:
	string name;
	int register_slot;
	size_t size;

	D3D11_USAGE usage;
};

class SamplerBuffer
{
public:
	SamplerBuffer(string paName, int slot_num)
		: name(paName), register_slot(slot_num) {}

public:
	string name;
	int register_slot;
};

class ShaderResourceBuffer
{
public:
	ShaderResourceBuffer(string paName, int slot_num)
		: name(paName), register_slot(slot_num) {}

public:
	string name;
	int register_slot;
};

