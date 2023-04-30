struct CacheBlock {
  bool valid;
  unsigned int tag;
};

unsigned int log2(unsigned int n) {
  unsigned int result = 0;
  while (n >>= 1) {
    result++;
  }
  return result;
}

unsigned int get_tag(unsigned int address, unsigned int index_bits,
                     unsigned int block_offset_bits) {
  return address >> (index_bits + block_offset_bits);
}

unsigned int get_index(unsigned int address, unsigned int index_bits,
                       unsigned int block_offset_bits) {
  return (address >> block_offset_bits) & ((1 << index_bits) - 1);
}