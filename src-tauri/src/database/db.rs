use std::error::Error;
use std::result::Result;

use mysql::prelude::Queryable;
use mysql::Pool;

#[allow(unused)]
pub fn connect() -> Result<(), Box<dyn Error>> {
    let url = "mysql://root:jestemkox1@localhost:3306/sentinel";
    let pool = Pool::new(url)?;

    let mut conn = pool.get_conn()?;

    conn.query_drop(
        r"
            CREATE TEMPORARY TABLE sentinel (

        )")?;

    Ok(())
}