#ifndef  PCM_H
#define  PCM_H










INT32S pcm_decode(INT16U *input,INT16U *output,INT32U len,INT16U chtype,INT16U wavtype);


INT32S pcm_encode(INT16U *input,INT16U *output,INT32U len,INT16U chtype,INT16U wavtype);



INT32S imaadpcm_decode(INT16U *input,INT16U *output,INT32U len,INT16U chtype,INT16U wavtype);

INT32S imaadpcm_encode(INT16U *input,INT16U *output,INT32U len,INT16U chtype,INT16U wavtype);

INT32S imadpcm_reset(void);



INT32S alaw_decode(INT16U *input,INT16U *output,INT32U len,INT16U chtype,INT16U wavtype);

INT32S alaw_encode(INT16U *input,INT16U *output,INT32U len,INT16U chtype,INT16U wavtype);


INT32S ulaw_decode(INT16U *input,INT16U *output,INT32U len,INT16U chtype,INT16U wavtype);

INT32S ulaw_encode(INT16U *input,INT16U *output,INT32U len,INT16U chtype,INT16U wavtype);

#endif