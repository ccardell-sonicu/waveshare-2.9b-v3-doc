
/**
 *  @brief: set the look-up table
 */
void edp_set_lut(void) {
    unsigned int count;     
    epd_2in9b_v3_send_command(LUT_FOR_VCOM);                            //vcom
    for(count = 0; count < 60; count++) {
        epd_2in9b_v3_send_data(lut_vcom0[count]);
    }
    
    epd_2in9b_v3_send_command(LUT_WHITE_TO_WHITE);                      //ww --
    for(count = 0; count < 36; count++) {
        epd_2in9b_v3_send_data(lut_ww[count]);
    }   
    
    epd_2in9b_v3_send_command(LUT_BLACK_TO_WHITE);                      //bw r
    for(count = 0; count < 60; count++) {
        epd_2in9b_v3_send_data(lut_bw[count]);
    } 

    epd_2in9b_v3_send_command(LUT_WHITE_TO_BLACK);                      //wb w
    for(count = 0; count < 36; count++) {
        epd_2in9b_v3_send_data(lut_wb[count]);
    } 

    epd_2in9b_v3_send_command(LUT_BLACK_TO_BLACK);                      //bb b
    for(count = 0; count < 36; count++) {
        epd_2in9b_v3_send_data(lut_bb[count]);
    } 
}

/**
 *  @brief: set the look-up table
 */
void edp_set_test_lut(void) {
    unsigned int count;     
    epd_2in9b_v3_send_command(LUT_FOR_VCOM);                            //vcom
    for(count = 0; count < 60; count++) {
        epd_2in9b_v3_send_data(lut_vcom0[count]);
    }
    
    epd_2in9b_v3_send_command(LUT_WHITE_TO_WHITE);                      //ww --
    for(count = 0; count < 36; count++) {
        epd_2in9b_v3_send_data(test_lut_ww[count]);
    }   
    
    epd_2in9b_v3_send_command(LUT_BLACK_TO_WHITE);                      //bw r
    for(count = 0; count < 60; count++) {
        epd_2in9b_v3_send_data(test_lut_bw[count]);
    } 

    epd_2in9b_v3_send_command(LUT_WHITE_TO_BLACK);                      //wb w
    for(count = 0; count < 36; count++) {
        epd_2in9b_v3_send_data(test_lut_wb[count]);
    } 

    epd_2in9b_v3_send_command(LUT_BLACK_TO_BLACK);                      //bb b
    for(count = 0; count < 36; count++) {
        epd_2in9b_v3_send_data(test_lut_bb[count]);
    } 
}
