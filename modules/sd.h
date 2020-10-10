 #ifndef UTIL_UTIL_SD_H_
#define UTIL_UTIL_SD_H_

extern bool util_sd_available();
extern uint32_t util_sd_file_size(const char *path);
extern FRESULT util_sd_load_file(const char *path, uint8_t *p_buffer, uint32_t count);
extern FRESULT util_sd_store_file(const char *path, uint8_t *p_buffer, uint32_t count);
extern bool util_sd_init();
extern uint16_t util_sd_read_16(FIL *p_file);
extern uint32_t util_sd_read_32(FIL *p_file);
extern bool util_sd_recover();
extern void util_sd_error();
extern uint8_t util_sd_getnum_files(const char *path, const char *extension);

#endif /* UTIL_UTIL_SD_H_ */
