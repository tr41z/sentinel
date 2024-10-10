CREATE TABLE IF NOT EXISTS flows (
    id SERIAL PRIMARY KEY,
    src_ip VARCHAR(45), 
    src_port INT,
    dst_ip VARCHAR(45),
    dst_port INT,
    protocol INT,
    total_bytes INT,
    total_packet_count INT,

    rate FLOAT,
    sbytes INT,
    dbytes INT,
    sload FLOAT,
    dload FLOAT,

    sttl INT,
    dttl INT,
    smean INT,

    start_time DATETIME,
    last_updated_time DATETIME,
    duration TEXT
);
