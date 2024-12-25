mod tests {
    use std::time::{Duration, SystemTime};

    use crate::database::db::timestamp_to_system_time;

    /* TIMESTAMP TO SYSTEM TIME */
    #[test]
    fn timestamp_to_system_time_epoch() {
        let timestamp: i64 = 0;
        let sys_time = timestamp_to_system_time(timestamp);
        let expected_time = SystemTime::UNIX_EPOCH;

        assert_eq!(sys_time, expected_time);
    }

    #[test]
    fn timestamp_to_system_time_large_value() {
        let timestamp: i64 = i64::MAX;
        let sys_time = timestamp_to_system_time(timestamp);
        let expected_time = SystemTime::UNIX_EPOCH + Duration::from_secs(i64::MAX as u64);

        assert_eq!(sys_time, expected_time);
    }

    #[test]
    #[should_panic(expected = "overflow when adding duration to instant")]
    fn timestamp_to_system_time_negative() {
        let timestamp: i64 = -1;
        let _ = timestamp_to_system_time(timestamp); // This should panic
    }
}
