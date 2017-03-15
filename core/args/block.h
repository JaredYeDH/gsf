#ifndef _GSF_BLOCK_HEADER_
#define _GSF_BLOCK_HEADER_

#include <memory>

namespace gsf
{
	// ��char*���¼��׷�װ��������պ�ʹ��

	struct Block
	{
		Block(uint32_t size)
			: size_(size)
		{
			buf_ = (char*)malloc(size_);
		}

		~Block()
		{
			free(buf_);
		}

		void push_msghead(uint32_t len, uint32_t msg_id)
		{
			// ����Ҫ�����´�С�˵����⡣
			
		}

		char *buf_;
		uint32_t size_;
	};

	typedef std::shared_ptr<Block> BlockPtr;
}

#endif
