//
// Aspia Project
// Copyright (C) 2018 Dmitry Chapyshev <dmitry@aspia.ru>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
//

#ifndef ASPIA_CODEC__VIDEO_DECODER_H_
#define ASPIA_CODEC__VIDEO_DECODER_H_

#include <memory>

#include "desktop_capture/desktop_frame.h"
#include "protocol/desktop_session.pb.h"

namespace aspia {

class VideoDecoder
{
public:
    virtual ~VideoDecoder() = default;

    static std::unique_ptr<VideoDecoder> create(proto::desktop::VideoEncoding encoding);

    virtual bool decode(const proto::desktop::VideoPacket& packet, DesktopFrame* frame) = 0;
};

} // namespace aspia

#endif // ASPIA_CODEC__VIDEO_DECODER_H_
