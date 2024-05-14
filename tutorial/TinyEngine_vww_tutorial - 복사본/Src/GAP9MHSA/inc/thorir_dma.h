
typedef struct
{
  void *ext;
  void *loc;
  unsigned short hwc_to_chw;
  unsigned short stride_2d;
  unsigned short number_of_2d_copies;
  unsigned short stride_1d;
  unsigned short number_of_1d_copies;
  unsigned short length_1d_copy;
  int dir; // 0 l1->l2, 1 l2->l1
} DMA_copy;
