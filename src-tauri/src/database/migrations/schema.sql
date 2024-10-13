CREATE TABLE IF NOT EXISTS flows (
    id SERIAL PRIMARY KEY,
    src_ip VARCHAR(45), 
    src_port INT,
    dst_ip VARCHAR(45),
    dst_port INT,
    protocol INT,
    total_bytes INT,
    total_packet_count INT,

    sbytes INT,
    smean INT,
    dmean INT,
    dbytes INT,
    dload FLOAT,
    sload FLOAT,
    dpkts INT,
    rate FLOAT,
    dttl INT,
    spkts INT,

    start_time DATETIME,
    last_updated_time DATETIME,
    dur FLOAT
);
