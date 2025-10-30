#include "ascii_num3.h"



//-------------------------ascii table----------------------
const unsigned char *ascii_num3_table[]=
{
   ascii_num3_32,// ' '
   ascii_num3_33,// '!'
   ascii_num3_35,// '#'
   ascii_num3_36,// '$'
   ascii_num3_37,// '%'
   ascii_num3_38,// '&'
   ascii_num3_39,// '''
   ascii_num3_40,// '('
   ascii_num3_41,// ')'
   ascii_num3_42,// '*'
   ascii_num3_43,// '+'
   ascii_num3_44,// ','
   ascii_num3_45,// '-'
   ascii_num3_46,// '.'
   ascii_num3_47,// '/'
   ascii_num3_48,// '0'
   ascii_num3_49,// '1'
   ascii_num3_50,// '2'
   ascii_num3_51,// '3'
   ascii_num3_52,// '4'
   ascii_num3_53,// '5'
   ascii_num3_54,// '6'
   ascii_num3_55,// '7'
   ascii_num3_56,// '8'
   ascii_num3_57,// '9'
   ascii_num3_58,// ':'
   ascii_num3_59,// ';'
   ascii_num3_60,// '<'
   ascii_num3_61,// '='
   ascii_num3_62,// '>'
   ascii_num3_63,// '?'
   ascii_num3_64,// '@'
   ascii_num3_65,// 'A'
   ascii_num3_66,// 'B'
   ascii_num3_67,// 'C'
   ascii_num3_68,// 'D'
   ascii_num3_69,// 'E'
   ascii_num3_70,// 'F'
   ascii_num3_71,// 'G'
   ascii_num3_72,// 'H'
   ascii_num3_73,// 'I'
   ascii_num3_74,// 'J'
   ascii_num3_75,// 'K'
   ascii_num3_76,// 'L'
   ascii_num3_77,// 'M'
   ascii_num3_78,// 'N'
   ascii_num3_79,// 'O'
   ascii_num3_80,// 'P'
   ascii_num3_81,// 'Q'
   ascii_num3_82,// 'R'
   ascii_num3_83,// 'S'
   ascii_num3_84,// 'T'
   ascii_num3_85,// 'U'
   ascii_num3_86,// 'V'
   ascii_num3_87,// 'W'
   ascii_num3_88,// 'X'
   ascii_num3_89,// 'Y'
   ascii_num3_90,// 'Z'
   ascii_num3_91,// '['
   ascii_num3_92,// '\'
   ascii_num3_93,// ']'
   ascii_num3_94,// '^'
   ascii_num3_95,// '_'
   ascii_num3_96,// '`'
   ascii_num3_97,// 'a'
   ascii_num3_98,// 'b'
   ascii_num3_99,// 'c'
   ascii_num3_100,// 'd'
   ascii_num3_101,// 'e'
   ascii_num3_102,// 'f'
   ascii_num3_103,// 'g'
   ascii_num3_104,// 'h'
   ascii_num3_105,// 'i'
   ascii_num3_106,// 'j'
   ascii_num3_107,// 'k'
   ascii_num3_108,// 'l'
   ascii_num3_109,// 'm'
   ascii_num3_110,// 'n'
   ascii_num3_111,// 'o'
   ascii_num3_112,// 'p'
   ascii_num3_113,// 'q'
   ascii_num3_114,// 'r'
   ascii_num3_115,// 's'
   ascii_num3_116,// 't'
   ascii_num3_117,// 'u'
   ascii_num3_118,// 'v'
   ascii_num3_119,// 'w'
   ascii_num3_120,// 'x'
   ascii_num3_121,// 'y'
   ascii_num3_122,// 'z'
   ascii_num3_123,// '{'
   ascii_num3_124,// '|'
   ascii_num3_125,// '}'
   ascii_num3_126,// '~'
};


const unsigned char *ascii_num3_get(char c,unsigned char *width,unsigned char *heigth)
{
   const unsigned char *table;
   unsigned char index;
   if(c<32 || c == 34 || c>126)
      return ((void *)0);
   if(c<34)
      index = c-32;
   else
      index = c-33;
   table = ascii_num3_table[index];
   if(width)
      *width = table[0];
   if(heigth)
      *heigth = table[1];
   return &table[2];
}
