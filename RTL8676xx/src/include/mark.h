/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ����� : nfmark.h
 �ļ����� : skb->nfmark bitλ����

 �޶���¼ :
          1 ���� : pengyao
            ���� : 2010-12-07
            ���� :
**********************************************************************/
#ifndef __NFMARK_H__
#define __NFMARK_H__


/*********************************************************************
 *                              mark��غ궨��                       *
 *********************************************************************/

/*
skb->nfmark 32bitλ����:
                            1111    //0 - 3bit����ClsIdx
                         111        //4 - 6bit����L2/L3/FW/APP/POST����
                        1           //7 bit�������������������
                    1111            //8 - 11bit����bridgeʵ�����
                1111                //12 - 15bit����dummyport�豸��ű��
           11111                    //16 - 20bit���ڲ���·�ɱ��
11111111111                         //21 - 31bitԤ��



0 - 3bit����ClsIdx
4 - 6bit����L2/L3/FW/APP/POST����
7 bit�������������������

8 - 11bit����bridgeʵ�����
12 - 15bit����dummy�豸��ű��
16 - 20bit���ڲ���·�ɱ��
21 - 31bitԤ��
*/

/*ClsIdxռ�õ�bitλ*/
#define CLASSIDX_NFMARK_START_BIT         (0)
#define CLASSIDX_NFMARK_END_BIT           (3)

/*L2/L3/FW/APP/POSTռ�õ�bitλ*/
#define TYPE_NFMARK_START_BIT             (4)
#define TYPE_NFMARK_END_BIT               (6)

/*�����б�ǵ�bitλ*/
#define UP_DOWN_NFMARK_BIT                (7)

/*bridge markռ�õ�bitλ*/
#define VLAN_BRID_MARK_START_BIT          (8)
#define VLAN_BRID_MARK_END_BIT            (11)

/*dummyport�豸���ռ�õ�bitλ*/
#define DUMMYPORTDEV_MARK_START_BIT       (12)
#define DUMMYPORTDEV_MARK_END_BIT         (15)

/*����·�ɱ��ռ�õ�bitλ*/
#define PROUTE_MARK_START_BIT             (16)
#define PROUTE_MARK_END_BIT               (20)

/*ClsIdx��Ӧ��mask*/
#define CLASSIDX_NFMARK_MASK \
    ((0xFFFFFFFFUL<<CLASSIDX_NFMARK_START_BIT) - (0xFFFFFFFFUL<<(CLASSIDX_NFMARK_END_BIT + 1)))

/*L2/L3/FW/APP/POST��Ӧ��mask*/
#define TYPE_NFMARK_MASK \
    ((0xFFFFFFFFUL<<TYPE_NFMARK_START_BIT) - (0xFFFFFFFFUL<<(TYPE_NFMARK_END_BIT + 1)))


/*������mark���mask*/
#define UP_DOWN_NFMARK_MASK     (0x1UL<<UP_DOWN_NFMARK_BIT)


/*bridgeʵ��id��Ӧ��mask*/
#define VLAN_BRKEY_MARK_MASK \
    ((0XFFFFFFFFUL<<VLAN_BRID_MARK_START_BIT) - (0XFFFFFFFFUL<<(VLAN_BRID_MARK_END_BIT + 1)))

/*ʮ����bridgeʵ��idת��:mask+mark��ʾ*/
#define VLAN_GET_BRKEY_MARK(iMark) \
        (VLAN_BRKEY_MARK_MASK&(iMark<<VLAN_BRID_MARK_START_BIT))


/*dummyport�豸��Ŷ�Ӧ��mask*/
#define DUMMYPORTDEV_MARK_MASK \
    ((0XFFFFFFFFUL<<DUMMYPORTDEV_MARK_START_BIT) - (0XFFFFFFFFUL<<(DUMMYPORTDEV_MARK_END_BIT + 1)))

/*ʮ����dummyport�豸���ת��:mask+mark��ʾ*/
#define DUMMYPORTDEV_MARK(iMark) \
        (DUMMYPORTDEV_MARK_MASK&(iMark<<DUMMYPORTDEV_MARK_START_BIT))


/*����·�ɱ��mark��Ӧ��mask*/
#define PROUTE_MARK_MASK \
    ((0XFFFFFFFFUL<<PROUTE_MARK_START_BIT) - (0XFFFFFFFFUL<<(PROUTE_MARK_END_BIT + 1)))

/* ����·��mark ��׼ֵ*/
#define PROUTE_MARK_BASE  200

/*ʮ���Ʋ���·�ɱ��ת��:mask+mark��ʾ����skb->mark��ȡ��ʱӦ����PROUTE_MARK_BASE*/
#define PROUTE_MARK(iMark) \
        (PROUTE_MARK_MASK&((iMark - PROUTE_MARK_BASE)<<PROUTE_MARK_START_BIT))

/*��skb->mark����ȡ����·�ɱ��*/
#define GET_PROUTE_MARK_BY_MASK(mark) \
        (((PROUTE_MARK_MASK&(mark))>>PROUTE_MARK_START_BIT) + PROUTE_MARK_BASE)

/******************************************************************************
 *                                 END                                        *
 ******************************************************************************/
#endif /* __NFMARK_H__ */

