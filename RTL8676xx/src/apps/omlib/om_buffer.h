#ifndef  __OM_BUFFER_H__
#define  __OM_BUFFER_H__

typedef struct tag_ST_OM_BLOCK
{
    char    *pBuffer;
    int        nSize;
    int     nStrLen;
}ST_OM_BLOCK;


#define OM_BUFF_NEW(n)   OM_BufferNew(n, __FILE__, __LINE__)
#define OM_BUFF_FREE(p)  {OM_BufferFree(p); p = NULL;}
/* ����һ��BUFFER���ߴ����Ϊ0 */
ST_OM_BLOCK    *OM_BufferNew(int nSize, char *szFile, int nLine);

/* ����һ��buffer */
ST_OM_BLOCK    *OM_BufferCopy(ST_OM_BLOCK *pBuffer);

/* �ͷ�buffer */
void            OM_BufferFree(ST_OM_BLOCK *pBuffer);

/* ��buf�ĳ�������Ϊ������min_sizeָ���ķ�Χ */
int                OM_BufferMinSize(ST_OM_BLOCK *pBuffer, int min_size);

/* ����һ���ı���buf�� */
int                OM_BufferInsTxt(ST_OM_BLOCK *pBuffer, int pos,
                        const char *new_text, int new_text_size, int eat);

#endif /* __OM_BUFFER_H__ */
