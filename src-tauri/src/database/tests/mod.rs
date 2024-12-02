mod tests {
    use std::time::{Duration, SystemTime};

    use crate::database::db::{system_time_to_timestamp, timestamp_to_system_time};
    use crate::database::model::DataModel;

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

    /* SYSTEM TIME TO TIMESTAMP */
    #[test]
    fn system_time_to_timestamp_epoch() {
        let time: SystemTime = SystemTime::UNIX_EPOCH;
        let converted_time = system_time_to_timestamp(time);

        assert_eq!(converted_time, 0);
    }

    #[test]
    fn system_time_to_timestamp_large_value() {
        let time: SystemTime = SystemTime::UNIX_EPOCH + Duration::from_secs(i64::MAX as u64);
        let converted_time = system_time_to_timestamp(time);

        assert_eq!(converted_time, i64::MAX);
    }

    #[test]
    fn system_time_to_timestamp_negative() {
        let time = SystemTime::UNIX_EPOCH - Duration::from_secs(1);
        let converted_time = system_time_to_timestamp(time);

        // Expecting fallback behavior since negatives aren't supported.
        assert_eq!(converted_time, 0);
    }

    /* CALCULATE RATE */
    #[test]
    fn calculate_rate_success() {
        let size: f64 = 24123.0;
        let duration: f64 = 13.0;

        let rate: f64 = DataModel::calculate_rate(size, duration);
        assert_eq!(rate, size / duration);
    }

    #[test]
    fn calculate_rate_zero_duration() {
        let size: f64 = 24123.0;
        let duration: f64 = 0.0;

        let rate: f64 = DataModel::calculate_rate(size, duration);
        assert_eq!(rate, 0.0);
    }

    #[test]
    fn calculate_rate_zero_size() {
        let size: f64 = 0.0;
        let duration: f64 = 12.0;

        let rate: f64 = DataModel::calculate_rate(size, duration);
        assert_eq!(rate, 0.0);
    }

    #[test]
    fn calculate_rate_zero_both() {
        let size: f64 = 0.0;
        let duration: f64 = 0.0;

        let rate: f64 = DataModel::calculate_rate(size, duration);
        assert_eq!(rate, 0.0)
    }

    #[test]
    fn calculate_rate_negative_size() {
        let size: f64 = -20.5;
        let duration: f64 = 12.0;

        let rate: f64 = DataModel::calculate_rate(size, duration);
        assert_eq!(rate, 0.0);
    }

    #[test]
    fn calculate_rate_negative_duration() {
        let size: f64 = 20142.0;
        let duration: f64 = -2.0;

        let rate: f64 = DataModel::calculate_rate(size, duration);
        assert_eq!(rate, 0.0);
    }

    #[test]
    fn calculate_rate_negative_both() {
        let size: f64 = -29.0;
        let duration: f64 = -10.0;

        let rate: f64 = DataModel::calculate_rate(size, duration);
        assert_eq!(rate, 0.0);
    }

    #[test]
    fn calculate_mean_success() {
        let size: u64 = 10;
        let total_packet_count: u64 = 3;

        let mean: u64 = DataModel::calculate_mean(size, total_packet_count);
        assert_eq!(mean, 3);
    }
}
