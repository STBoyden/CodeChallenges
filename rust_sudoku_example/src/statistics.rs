use std::{fmt, time::Duration};

pub struct Statistics {
    pub is_solved: bool,
    pub num_assignments: i32,
    pub time_elapsed: Duration,
}

impl Statistics {
    pub fn new(is_solved: bool, num_assignments: i32, time_elapsed: Duration) -> Self {
        Self {
            is_solved,
            num_assignments,
            time_elapsed,
        }
    }
}

impl fmt::Display for Statistics {
    fn fmt(&self, formatter: &mut fmt::Formatter) -> fmt::Result {
        return formatter.write_str(
            format!(
                "Solved: {}
Assignments: {}
Time elapsed: {}s",
                self.is_solved,
                self.num_assignments,
                self.time_elapsed.as_secs_f64()
            )
            .as_str(),
        );
    }
}
