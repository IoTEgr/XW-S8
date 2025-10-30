#include "ascii_num4.h"



//-------------------------ascii table----------------------
const unsigned char *ascii_num4_table[]=
{
   ascii_num4_32,// ' '
   ascii_num4_33,// '!'
   ascii_num4_35,// '#'
   ascii_num4_36,// '$'
   ascii_num4_37,// '%'
   ascii_num4_38,// '&'
   ascii_num4_39,// '''
   ascii_num4_40,// '('
   ascii_num4_41,// ')'
   ascii_num4_42,// '*'
   ascii_num4_43,// '+'
   ascii_num4_44,// ','
   ascii_num4_45,// '-'
   ascii_num4_46,// '.'
   ascii_num4_47,// '/'
   ascii_num4_48,// '0'
   ascii_num4_49,// '1'
   ascii_num4_50,// '2'
   ascii_num4_51,// '3'
   ascii_num4_52,// '4'
   ascii_num4_53,// '5'
   ascii_num4_54,// '6'
   ascii_num4_55,// '7'
   ascii_num4_56,// '8'
   ascii_num4_57,// '9'
   ascii_num4_58,// ':'
   ascii_num4_59,// ';'
   ascii_num4_60,// '<'
   ascii_num4_61,// '='
   ascii_num4_62,// '>'
   ascii_num4_63,// '?'
   ascii_num4_64,// '@'
   ascii_num4_65,// 'A'
   ascii_num4_66,// 'B'
   ascii_num4_67,// 'C'
   ascii_num4_68,// 'D'
   ascii_num4_69,// 'E'
   ascii_num4_70,// 'F'
   ascii_num4_71,// 'G'
   ascii_num4_72,// 'H'
   ascii_num4_73,// 'I'
   ascii_num4_74,// 'J'
   ascii_num4_75,// 'K'
   ascii_num4_76,// 'L'
   ascii_num4_77,// 'M'
   ascii_num4_78,// 'N'
   ascii_num4_79,// 'O'
   ascii_num4_80,// 'P'
   ascii_num4_81,// 'Q'
   ascii_num4_82,// 'R'
   ascii_num4_83,// 'S'
   ascii_num4_84,// 'T'
   ascii_num4_85,// 'U'
   ascii_num4_86,// 'V'
   ascii_num4_87,// 'W'
   ascii_num4_88,// 'X'
   ascii_num4_89,// 'Y'
   ascii_num4_90,// 'Z'
   ascii_num4_91,// '['
   ascii_num4_92,// '\'
   ascii_num4_93,// ']'
   ascii_num4_94,// '^'
   ascii_num4_95,// '_'
   ascii_num4_96,// '`'
   ascii_num4_97,// 'a'
   ascii_num4_98,// 'b'
   ascii_num4_99,// 'c'
   ascii_num4_100,// 'd'
   ascii_num4_101,// 'e'
   ascii_num4_102,// 'f'
   ascii_num4_103,// 'g'
   ascii_num4_104,// 'h'
   ascii_num4_105,// 'i'
   ascii_num4_106,// 'j'
   ascii_num4_107,// 'k'
   ascii_num4_108,// 'l'
   ascii_num4_109,// 'm'
   ascii_num4_110,// 'n'
   ascii_num4_111,// 'o'
   ascii_num4_112,// 'p'
   ascii_num4_113,// 'q'
   ascii_num4_114,// 'r'
   ascii_num4_115,// 's'
   ascii_num4_116,// 't'
   ascii_num4_117,// 'u'
   ascii_num4_118,// 'v'
   ascii_num4_119,// 'w'
   ascii_num4_120,// 'x'
   ascii_num4_121,// 'y'
   ascii_num4_122,// 'z'
   ascii_num4_123,// '{'
   ascii_num4_124,// '|'
   ascii_num4_125,// '}'
   ascii_num4_126,// '~'
};


const unsigned char *ascii_num4_get(char c,unsigned char *width,unsigned char *heigth)
{
   const unsigned char *table;
   unsigned char index;
   if(c<32 || c == 34 || c>126)
      return ((void *)0);
   if(c<34)
      index = c-32;
   else
      index = c-33;
   table = ascii_num4_table[index];
   if(width)
      *width = table[0];
   if(heigth)
      *heigth = table[1];
   return &table[2];
}
