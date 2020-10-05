pub mod board {
    pub const BOARD_HEIGHT: i32 = 9;
    pub const BOARD_WIDTH: i32 = 9;
    pub const CLIQUE_HEIGHT: i32 = 3;
    pub const CLIQUE_WIDTH: i32 = 3;
    pub const NUM_CLIQUES: i32 = (BOARD_HEIGHT * BOARD_WIDTH) / (CLIQUE_HEIGHT * CLIQUE_WIDTH);
}

pub mod border {
    pub const DOUBLE_VERTICAL: &str = "║";
    pub const DOUBLE_HORIZONTAL: &str = "═";
    pub const DOUBLE_NE: &str = "╗";
    pub const DOUBLE_NW: &str = "╔";
    pub const DOUBLE_SW: &str = "╚";
    pub const DOUBLE_SE: &str = "╝";
    pub const DOUBLE_SINGLE_S: &str = "╤";
    pub const DOUBLE_SINGLE_E: &str = "╟";
    pub const DOUBLE_SINGLE_N: &str = "╧";
    pub const DOUBLE_SINGLE_W: &str = "╢";
    pub const SINGLE_VERTICAL: &str = "│";
    pub const SINGLE_HORIZONTAL: &str = "─";
    pub const SINGLE_CROSS: &str = "┼";
}

pub mod printing {
    pub const SLEEP_DURATION: f32 = 0.05;
    pub const BOARD_HEIGHT: i32 = 13;
    pub const CLIQUE_SIZE: i32 = 7;
}
