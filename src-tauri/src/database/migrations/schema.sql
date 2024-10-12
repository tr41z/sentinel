CREATE TABLE IF NOT EXISTS flows (
    id SERIAL PRIMARY KEY,
    src_ip VARCHAR(45), 
    src_port INT,
    dst_ip VARCHAR(45),
    dst_port INT,
    protocol INT,

    max_fwd_packet_length INT,
    fwd_packet_length_mean	 FLOAT,
    init_win_bytes_forward INT,

    start_time DATETIME,
    last_updated_time DATETIME,
    duration FLOAT
);
