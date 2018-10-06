// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len >= 0)
	{
		BitLen = len;
		MemLen = len / (sizeof(TELEM) * 8);
		if ((len % (sizeof(TELEM) * 8)) > 0)
			MemLen++;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
			pMem[i] = 0;
	}
	else
		throw "Error lenght < 0";
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[bf.MemLen];
	for (int i = 0; i < bf.MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n >= 0)
	{
		int Ind = n / (sizeof(TELEM) * 8);
		if ((n % (sizeof(TELEM) * 8)) > 0)
			Ind++;
		return Ind;
	}
	else
		throw "Wrong index";
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n >= 0 && n < BitLen)
	{
		TELEM mask = 1;
		int BitPos = (n % (sizeof(TELEM) * 8));
		mask = mask << BitPos;
		return mask;
	}
	else
		throw "Wrong index";
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= 0 && n < BitLen)
	{
		int Ind = n / (sizeof(TELEM) * 8);
		pMem[Ind] = pMem[Ind] | GetMemMask(n);
	}
	else
		throw "Wrong index";
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= 0 && n < BitLen)
	{
		int Ind = n / (sizeof(TELEM) * 8);
		pMem[Ind] = pMem[Ind] & ~GetMemMask(n);
	}
	else
		throw "Wrong index";
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n >= 0) && (n <= BitLen))
	{
		int Ind = n / (sizeof(TELEM) * 8);
		if ((pMem[Ind] & GetMemMask(n)) == GetMemMask(n))
			return 1;
		else
			return 0;
	}
	else
		throw "Wrong index";
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (BitLen != bf.BitLen)
	{
		delete[] pMem;
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
	}
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (this->MemLen == bf.MemLen)
	{
		int count = 0;
		for (int i = 0; i < MemLen; i++)
			if (this->pMem[i] == bf.pMem[i])
				count++;
		if (count == MemLen)
			return 1;
		else return 0;
	}
	else return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (this->MemLen == bf.MemLen)
	{
		int count = 0;
		for (int i = 0; i < MemLen; i++)
			if (this->pMem[i] == bf.pMem[i])
				count++;
		if (count == MemLen)
			return 0;
		else return 1;
	}
	else return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField tmp(bf.BitLen);
	for (int i = 0; i < this->MemLen; i++)
		tmp.pMem[i] = this->pMem[i] | bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField tmp(bf.BitLen);
	for (int i = 0; i < this->MemLen; i++)
		tmp.pMem[i] = this->pMem[i] & bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);
	for (int i = 0; i < BitLen; i++)
	{
		if (GetBit(i) == 0)
			tmp.SetBit(i);
	}
	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int count = 0;
	string str;
	str = getchar();
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == ' ')
			count++;
		if (str[i] == '1')
			bf.SetBit(i - count);
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		int Ind = i / (sizeof(TELEM) * 8);
		if ((bf.pMem[Ind] & bf.GetMemMask(i)) == bf.GetMemMask(i))
			ostr << "1";
		else
			ostr << "0";
	}
	return ostr;
}
