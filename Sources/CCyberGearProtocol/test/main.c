//
//  main.c
//
//  Created by Eric Wu on 2023/9/4.
//

#include <stdio.h>
#include <string.h>
#include "cyber_gear_protocol.h"

int main(int argc, const char * argv[]) {
    cyber_gear_can_t frame;
    cyber_gear_can_init(&frame);
    cyber_gear_set_can_id_host_can_id(&frame, 1);
    cyber_gear_set_can_id_target_can_id(&frame, 127);
    
    printf("Fetch current motor status\n");
    cyber_gear_set_can_id_communication_type(&frame, COMMUNICATION_FETCH_DEVICE_ID);
    cyber_gear_can_dump(&frame);
    
    printf("Set Run mode I\n");
    bzero(frame.can_data.bytes, 8);
    cyber_gear_build_parameter_write_frame_with_int_value(&frame, PARAMETER_RUN_MODE, 2);
    cyber_gear_can_dump(&frame);
    
    printf("Enable Motor\n");
    bzero(frame.can_data.bytes, 8);
    cyber_gear_set_can_id_communication_type(&frame, COMMUNICATION_ENABLE_DEVICE);
    cyber_gear_can_dump(&frame);
    
    printf("Set SPD_REF\n");
    bzero(frame.can_data.bytes, 8);
    cyber_gear_build_parameter_write_frame_with_float_value(&frame, PARAMETER_SPD_REF, 30.0);
    cyber_gear_can_dump(&frame);    
    return 0;
}
