#include "ascii_default.h"



//-------------------------ascii table----------------------
const unsigned char *ascii_default_table[]=
{
   ascii_default_32,// ' '
   ascii_default_33,// '!'
   ascii_default_35,// '#'
   ascii_default_36,// '$'
   ascii_default_37,// '%'
   ascii_default_38,// '&'
   ascii_default_39,// '''
   ascii_default_40,// '('
   ascii_default_41,// ')'
   ascii_default_42,// '*'
   ascii_default_43,// '+'
   ascii_default_44,// ','
   ascii_default_45,// '-'
   ascii_default_46,// '.'
   ascii_default_47,// '/'
   ascii_default_48,// '0'
   ascii_default_49,// '1'
   ascii_default_50,// '2'
   ascii_default_51,// '3'
   ascii_default_52,// '4'
   ascii_default_53,// '5'
   ascii_default_54,// '6'
   ascii_default_55,// '7'
   ascii_default_56,// '8'
   ascii_default_57,// '9'
   ascii_default_58,// ':'
   ascii_default_59,// ';'
   ascii_default_60,// '<'
   ascii_default_61,// '='
   ascii_default_62,// '>'
   ascii_default_63,// '?'
   ascii_default_64,// '@'
   ascii_default_65,// 'A'
   ascii_default_66,// 'B'
   ascii_default_67,// 'C'
   ascii_default_68,// 'D'
   ascii_default_69,// 'E'
   ascii_default_70,// 'F'
   ascii_default_71,// 'G'
   ascii_default_72,// 'H'
   ascii_default_73,// 'I'
   ascii_default_74,// 'J'
   ascii_default_75,// 'K'
   ascii_default_76,// 'L'
   ascii_default_77,// 'M'
   ascii_default_78,// 'N'
   ascii_default_79,// 'O'
   ascii_default_80,// 'P'
   ascii_default_81,// 'Q'
   ascii_default_82,// 'R'
   ascii_default_83,// 'S'
   ascii_default_84,// 'T'
   ascii_default_85,// 'U'
   ascii_default_86,// 'V'
   ascii_default_87,// 'W'
   ascii_default_88,// 'X'
   ascii_default_89,// 'Y'
   ascii_default_90,// 'Z'
   ascii_default_91,// '['
   ascii_default_92,// '\'
   ascii_default_93,// ']'
   ascii_default_94,// '^'
   ascii_default_95,// '_'
   ascii_default_96,// '`'
   ascii_default_97,// 'a'
   ascii_default_98,// 'b'
   ascii_default_99,// 'c'
   ascii_default_100,// 'd'
   ascii_default_101,// 'e'
   ascii_default_102,// 'f'
   ascii_default_103,// 'g'
   ascii_default_104,// 'h'
   ascii_default_105,// 'i'
   ascii_default_106,// 'j'
   ascii_default_107,// 'k'
   ascii_default_108,// 'l'
   ascii_default_109,// 'm'
   ascii_default_110,// 'n'
   ascii_default_111,// 'o'
   ascii_default_112,// 'p'
   ascii_default_113,// 'q'
   ascii_default_114,// 'r'
   ascii_default_115,// 's'
   ascii_default_116,// 't'
   ascii_default_117,// 'u'
   ascii_default_118,// 'v'
   ascii_default_119,// 'w'
   ascii_default_120,// 'x'
   ascii_default_121,// 'y'
   ascii_default_122,// 'z'
   ascii_default_123,// '{'
   ascii_default_124,// '|'
   ascii_default_125,// '}'
   ascii_default_126,// '~'
};


const unsigned char *ascii_default_get(char c,unsigned char *width,unsigned char *heigth)
{
   const unsigned char *table;
   unsigned char index;
   if(c<32 || c == 34 || c>126)
      return ((void *)0);
   if(c<34)
      index = c-32;
   else
      index = c-33;
   table = ascii_default_table[index];
   if(width)
      *width = table[0];
   if(heigth)
      *heigth = table[1];
   return &table[2];
}
