#pragma once

namespace Eng {
	struct Image {
		int width;
		int height;
		int num_channels_in_file;
		unsigned char* data;
	};
}