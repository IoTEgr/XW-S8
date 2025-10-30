#include "ascii_num5.h"

#if 0

//-------------------------ascii table----------------------
const unsigned char *ascii_num5_table[]=
{
   ascii_num5_32,// ' '
   ascii_num5_33,// '!'
   ascii_num5_35,// '#'
   ascii_num5_36,// '$'
   ascii_num5_37,// '%'
   ascii_num5_38,// '&'
   ascii_num5_39,// '''
   ascii_num5_40,// '('
   ascii_num5_41,// ')'
   ascii_num5_42,// '*'
   ascii_num5_43,// '+'
   ascii_num5_44,// ','
   ascii_num5_45,// '-'
   ascii_num5_46,// '.'
   ascii_num5_47,// '/'
   ascii_num5_48,// '0'
   ascii_num5_49,// '1'
   ascii_num5_50,// '2'
   ascii_num5_51,// '3'
   ascii_num5_52,// '4'
   ascii_num5_53,// '5'
   ascii_num5_54,// '6'
   ascii_num5_55,// '7'
   ascii_num5_56,// '8'
   ascii_num5_57,// '9'
   ascii_num5_58,// ':'
   ascii_num5_59,// ';'
   ascii_num5_60,// '<'
   ascii_num5_61,// '='
   ascii_num5_62,// '>'
   ascii_num5_63,// '?'
   ascii_num5_64,// '@'
   ascii_num5_65,// 'A'
   ascii_num5_66,// 'B'
   ascii_num5_67,// 'C'
   ascii_num5_68,// 'D'
   ascii_num5_69,// 'E'
   ascii_num5_70,// 'F'
   ascii_num5_71,// 'G'
   ascii_num5_72,// 'H'
   ascii_num5_73,// 'I'
   ascii_num5_74,// 'J'
   ascii_num5_75,// 'K'
   ascii_num5_76,// 'L'
   ascii_num5_77,// 'M'
   ascii_num5_78,// 'N'
   ascii_num5_79,// 'O'
   ascii_num5_80,// 'P'
   ascii_num5_81,// 'Q'
   ascii_num5_82,// 'R'
   ascii_num5_83,// 'S'
   ascii_num5_84,// 'T'
   ascii_num5_85,// 'U'
   ascii_num5_86,// 'V'
   ascii_num5_87,// 'W'
   ascii_num5_88,// 'X'
   ascii_num5_89,// 'Y'
   ascii_num5_90,// 'Z'
   ascii_num5_91,// '['
   ascii_num5_92,// '\'
   ascii_num5_93,// ']'
   ascii_num5_94,// '^'
   ascii_num5_95,// '_'
   ascii_num5_96,// '`'
   ascii_num5_97,// 'a'
   ascii_num5_98,// 'b'
   ascii_num5_99,// 'c'
   ascii_num5_100,// 'd'
   ascii_num5_101,// 'e'
   ascii_num5_102,// 'f'
   ascii_num5_103,// 'g'
   ascii_num5_104,// 'h'
   ascii_num5_105,// 'i'
   ascii_num5_106,// 'j'
   ascii_num5_107,// 'k'
   ascii_num5_108,// 'l'
   ascii_num5_109,// 'm'
   ascii_num5_110,// 'n'
   ascii_num5_111,// 'o'
   ascii_num5_112,// 'p'
   ascii_num5_113,// 'q'
   ascii_num5_114,// 'r'
   ascii_num5_115,// 's'
   ascii_num5_116,// 't'
   ascii_num5_117,// 'u'
   ascii_num5_118,// 'v'
   ascii_num5_119,// 'w'
   ascii_num5_120,// 'x'
   ascii_num5_121,// 'y'
   ascii_num5_122,// 'z'
   ascii_num5_123,// '{'
   ascii_num5_124,// '|'
   ascii_num5_125,// '}'
   ascii_num5_126,// '~'
};


const unsigned char *ascii_num5_get(char c,unsigned char *width,unsigned char *heigth)
{
   const unsigned char *table;
   unsigned char index;
   if(c<32 || c == 34 || c>126)
      return ((void *)0);
   if(c<34)
      index = c-32;
   else
      index = c-33;
   table = ascii_num5_table[index];
   if(width)
      *width = table[0];
   if(heigth)
      *heigth = table[1];
   return &table[2];
}

#endif

