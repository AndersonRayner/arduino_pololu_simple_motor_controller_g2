// Data structures for the spool drive

// === Structs
// Spool Drive Struct
struct spool_drive_struct 
{
    uint8_t  header;
     int16_t drive_speed; 
    uint8_t  checksum;
}__attribute__((packed));

const static int SPOOL_DRIVE_STRUCT_SIZE = sizeof(spool_drive_struct);

union spool_drive_data
{
    spool_drive_struct msg;
    uint8_t msgData[SPOOL_DRIVE_STRUCT_SIZE];
};

// Spool Brake Struct
struct spool_brake_struct 
{
    uint8_t  header;
    uint8_t brake_force; // Should be between 0 (coast) and 0x20 (full brake)
    uint8_t  checksum;
}__attribute__((packed));

const static int SPOOL_BRAKE_STRUCT_SIZE = sizeof(spool_brake_struct);

union spool_brake_data
{
    spool_brake_struct msg;
    uint8_t msgData[SPOOL_BRAKE_STRUCT_SIZE];
};

// Spool Command Struct
struct spool_command_struct 
{
    uint8_t command;
    uint8_t  checksum;
}__attribute__((packed));

const static int SPOOL_COMMAND_STRUCT_SIZE = sizeof(spool_command_struct);

union spool_command_data
{
    spool_command_struct msg;
    uint8_t msgData[SPOOL_COMMAND_STRUCT_SIZE];
};
