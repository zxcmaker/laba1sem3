// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

enum EXCEPTION { outofrange };

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
  if (len <= 0)
    throw length_error("TBitField object have different size");

  BitLen = len;
  if (len % 32 == 0)
  {
    MemLen = len / 32;
  }
  else
  {
    MemLen = len / 32;
  }

  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++)
    pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];

  for (int i = 0; i < MemLen; i++)
    pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
  delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  if (n >= BitLen || n < 0)
    throw outofrange;
  int res;
  res = n / 32;
  return (res);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  if (n >= BitLen || n < 0)
    throw outofrange;
  TELEM res;
  res = 1 << (n % 32);
  return res;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const noexcept // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  if (n >= BitLen)
    throw out_of_range("bit pos is out of range");
  pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if (n >= BitLen)
    throw out_of_range("bit pos is out of range");
  pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if (n >= BitLen || n < 0)
    throw length_error("TBitField object have different size");

  int i = GetMemIndex(n);
  TELEM mask = GetMemMask(n);
  int res = pMem[i] & mask;
  return res;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
  int i = 0;
  if (this != &bf)
  {
    delete[] pMem;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (i = 0; i < MemLen; i++)
    {
      pMem[i] = bf.pMem[i];
    }
  }
  return (*this);
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
  if (BitLen != bf.BitLen)
    return false;

  for (int i = 0; i < MemLen; i++)
    if (pMem[i] != bf.pMem[i])
      return false;
  return true;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
  if (BitLen != bf.BitLen)
    throw length_error("TBitField object have different size");

  TBitField tmp(*this);
  for (int i = 0; i < bf.MemLen; i++)
    tmp.pMem[i] |= bf.pMem[i];

  return tmp;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
  if(BitLen != bf.BitLen)
    throw length_error("TBitField object have different size");
  TBitField tmp(*this);
  for (int i = 0; i < bf.MemLen; i++)
    tmp.pMem[i] &= bf.pMem[i];

  return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
  for (int i = 0; i < BitLen; i++)
  {
    if (GetBit(i) == 0)
      SetBit(i);
    else
      ClrBit(i);
  }
  return (*this);
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
  char x;
  int i;
  for (i = 0; i < bf.BitLen; i++)
  {
    istr >> x;
    if (x == '0')
    {
      bf.ClrBit(i);
    }
    else
    {
      if (x == '1')
      {
        bf.SetBit(i);
      }
      else
        return istr;
    }
  }
  return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
  int i;
  for (i = 0; i < bf.BitLen; i++)
  {
    if (bf.GetBit(i))
    {
      ostr << '1';
    }
    else
    {
      ostr << '0';
    }
  }
  return ostr;
}
