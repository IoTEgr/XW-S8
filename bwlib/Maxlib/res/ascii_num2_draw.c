#include "ascii_num2.h"



//-------------------------ascii table----------------------
const unsigned char *ascii_num2_table[]=
{
   ascii_num2_32,// ' '
   ascii_num2_33,// '!'
   ascii_num2_35,// '#'
   ascii_num2_36,// '$'
   ascii_num2_37,// '%'
   ascii_num2_38,// '&'
   ascii_num2_39,// '''
   ascii_num2_40,// '('
   ascii_num2_41,// ')'
   ascii_num2_42,// '*'
   ascii_num2_43,// '+'
   ascii_num2_44,// ','
   ascii_num2_45,// '-'
   ascii_num2_46,// '.'
   ascii_num2_47,// '/'
   ascii_num2_48,// '0'
   ascii_num2_49,// '1'
   ascii_num2_50,// '2'
   ascii_num2_51,// '3'
   ascii_num2_52,// '4'
   ascii_num2_53,// '5'
   ascii_num2_54,// '6'
   ascii_num2_55,// '7'
   ascii_num2_56,// '8'
   ascii_num2_57,// '9'
   ascii_num2_58,// ':'
   ascii_num2_59,// ';'
   ascii_num2_60,// '<'
   ascii_num2_61,// '='
   ascii_num2_62,// '>'
   ascii_num2_63,// '?'
   ascii_num2_64,// '@'
   ascii_num2_65,// 'A'
   ascii_num2_66,// 'B'
   ascii_num2_67,// 'C'
   ascii_num2_68,// 'D'
   ascii_num2_69,// 'E'
   ascii_num2_70,// 'F'
   ascii_num2_71,// 'G'
   ascii_num2_72,// 'H'
   ascii_num2_73,// 'I'
   ascii_num2_74,// 'J'
   ascii_num2_75,// 'K'
   ascii_num2_76,// 'L'
   ascii_num2_77,// 'M'
   ascii_num2_78,// 'N'
   ascii_num2_79,// 'O'
   ascii_num2_80,// 'P'
   ascii_num2_81,// 'Q'
   ascii_num2_82,// 'R'
   ascii_num2_83,// 'S'
   ascii_num2_84,// 'T'
   ascii_num2_85,// 'U'
   ascii_num2_86,// 'V'
   ascii_num2_87,// 'W'
   ascii_num2_88,// 'X'
   ascii_num2_89,// 'Y'
   ascii_num2_90,// 'Z'
   ascii_num2_91,// '['
   ascii_num2_92,// '\'
   ascii_num2_93,// ']'
   ascii_num2_94,// '^'
   ascii_num2_95,// '_'
   ascii_num2_96,// '`'
   ascii_num2_97,// 'a'
   ascii_num2_98,// 'b'
   ascii_num2_99,// 'c'
   ascii_num2_100,// 'd'
   ascii_num2_101,// 'e'
   ascii_num2_102,// 'f'
   ascii_num2_103,// 'g'
   ascii_num2_104,// 'h'
   ascii_num2_105,// 'i'
   ascii_num2_106,// 'j'
   ascii_num2_107,// 'k'
   ascii_num2_108,// 'l'
   ascii_num2_109,// 'm'
   ascii_num2_110,// 'n'
   ascii_num2_111,// 'o'
   ascii_num2_112,// 'p'
   ascii_num2_113,// 'q'
   ascii_num2_114,// 'r'
   ascii_num2_115,// 's'
   ascii_num2_116,// 't'
   ascii_num2_117,// 'u'
   ascii_num2_118,// 'v'
   ascii_num2_119,// 'w'
   ascii_num2_120,// 'x'
   ascii_num2_121,// 'y'
   ascii_num2_122,// 'z'
   ascii_num2_123,// '{'
   ascii_num2_124,// '|'
   ascii_num2_125,// '}'
   ascii_num2_126,// '~'
};


const unsigned char *ascii_num2_get(char c,unsigned char *width,unsigned char *heigth)
{
   const unsigned char *table;
   unsigned char index;
   if(c<32 || c == 34 || c>126)
      return ((void *)0);
   if(c<34)
      index = c-32;
   else
      index = c-33;
   table = ascii_num2_table[index];
   if(width)
      *width = table[0];
   if(heigth)
      *heigth = table[1];
   return &table[2];
}
