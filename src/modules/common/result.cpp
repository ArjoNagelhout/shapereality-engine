//
// Created by Arjo Nagelhout on 19/03/2024.
//

#include "result.h"

namespace common
{
    std::string_view toString(ResultCode code)
    {
        switch (code)
        {
            case ResultCode::Success: return "Success";
            case ResultCode::Cancelled: return "Cancelled";
            case ResultCode::Unknown: return "Unknown";
            case ResultCode::InvalidArgument: return "InvalidArgument";
            case ResultCode::DeadlineExceeded: return "DeadlineExceeded";
            case ResultCode::NotFound: return "NotFound";
            case ResultCode::AlreadyExists: return "AlreadyExists";
            case ResultCode::PermissionDenied: return "PermissionDenied";
            case ResultCode::ResourceExhausted: return "ResourceExhausted";
            case ResultCode::FailedPrecondition: return "FailedPrecondition";
            case ResultCode::Aborted: return "Aborted";
            case ResultCode::OutOfRange: return "OutOfRange";
            case ResultCode::Unimplemented: return "Unimplemented";
            case ResultCode::Internal: return "Internal";
            case ResultCode::Unavailable: return "Unavailable";
            case ResultCode::DataLoss: return "DataLoss";
            case ResultCode::Unauthenticated: return "Unauthenticated";
        }
    }
}