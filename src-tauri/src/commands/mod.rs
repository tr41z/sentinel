pub mod commands {
    use sqlx::{Pool, Sqlite};

    use crate::database::db::get_all_flows;
    use crate::database::model::DataModel;

    // Retrieve all flows (async)
    pub async fn get_flows(pool: Pool<Sqlite>) -> Result<Vec<DataModel>, sqlx::Error> {
        let flows = get_all_flows(&pool).await?; // fetch flows from the database
        Ok(flows)
    }
}
