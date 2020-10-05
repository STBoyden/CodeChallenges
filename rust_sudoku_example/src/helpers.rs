use crate::consts::board::*;
use std::{collections::HashMap, sync::Mutex};

lazy_static! {
    pub static ref BOARDHELPER: Mutex<BoardHelper> = Mutex::new(BoardHelper::new());
}

pub struct BoardHelper {
    pub cliques: HashMap<i32, Vec<(i32, i32)>>,
}

impl BoardHelper {
    pub fn new() -> Self {
        Self {
            cliques: HashMap::new(),
        }
    }

    pub fn init(&mut self) {
        for y in 0..BOARD_HEIGHT {
            for x in 0..BOARD_WIDTH {
                let index = BoardHelper::get_index_for_coords((x, y));

                if !self.cliques.contains_key(&index) {
                    self.cliques.insert(index, Vec::new());
                }

                self.cliques.get_mut(&index).unwrap().push((x, y));
            }
        }
    }

    pub fn get_index_for_coords(coords: (i32, i32)) -> i32 {
        return {
            let meta_x = coords.0 / CLIQUE_WIDTH;
            let meta_y = coords.1 / CLIQUE_HEIGHT;

            (meta_y * CLIQUE_HEIGHT) + meta_x
        };
    }
}
