use crate::{consts::*, helpers::*};
use std::{collections::HashMap, fmt};

pub struct Board {
    pub raw_table: Vec<Vec<i32>>,
    pub editable_indices: Vec<(i32, i32)>,
}

impl Board {
    pub fn new(raw_table: &Vec<Vec<i32>>) -> Result<Self, String> {
        if !raw_table.len() == board::BOARD_HEIGHT as usize {
            return Err("Raw table height is incorrect".to_string());
        }

        if !raw_table.len() == board::BOARD_WIDTH as usize {
            return Err("Raw table width is incorrect!".to_string());
        }

        let mut editable_indices = vec![];

        for y in 0..board::BOARD_HEIGHT {
            for x in 0..board::BOARD_WIDTH {
                if raw_table[y as usize][x as usize] == 0 {
                    editable_indices.push((x, y));
                }
            }
        }

        Ok(Board {
            raw_table: raw_table.clone(),
            editable_indices,
        })
    }

    pub fn is_square_valid(&self, editable_indices_index: usize) -> bool {
        let (x, y) = self.editable_indices[editable_indices_index];

        if !self.is_row_valid(y) {
            return false;
        }

        if !self.is_column_valid(x) {
            return false;
        }

        let clique_index = BoardHelper::get_index_for_coords((x, y));
        if !self.is_clique_valid(clique_index as usize) {
            return false;
        }

        true
    }

    pub fn is_row_valid(&self, y: i32) -> bool {
        let mut occ_dict = HashMap::new();

        for x in 0..board::BOARD_WIDTH {
            let value = self.raw_table[y as usize][x as usize];

            if value == 0 {
                continue;
            }

            if occ_dict.contains_key(&value) {
                return false;
            } else {
                occ_dict.insert(value, 0);
            }
        }

        true
    }

    pub fn is_column_valid(&self, x: i32) -> bool {
        let mut occ_dict = HashMap::new();

        for y in 0..board::BOARD_HEIGHT {
            let value = self.raw_table[y as usize][x as usize];

            if value == 0 {
                continue;
            }

            if occ_dict.contains_key(&value) {
                return false;
            } else {
                occ_dict.insert(value, 0);
            }
        }

        true
    }

    pub fn is_clique_valid(&self, clique_index: usize) -> bool {
        let mut occ_dict = HashMap::new();

        let lock = BOARDHELPER.lock().unwrap();
        for (cx, cy) in lock
            .cliques
            .clone()
            .get(&(clique_index as i32))
            .unwrap()
            .clone()
        {
            let value = self.raw_table[cy as usize][cx as usize];

            if value == 0 {
                continue;
            }

            if occ_dict.contains_key(&value) {
                return false;
            } else {
                occ_dict.insert(value, 0);
            }
        }

        true
    }

    pub fn is_board_valid(&self) -> bool {
        for y in 0..board::BOARD_HEIGHT {
            if !self.is_row_valid(y) {
                return false;
            }
        }

        for x in 0..board::BOARD_WIDTH {
            if !self.is_column_valid(x) {
                return false;
            }
        }

        for clique_index in 0..board::NUM_CLIQUES {
            if !self.is_column_valid(clique_index) {
                return false;
            }
        }

        true
    }
}

impl fmt::Display for Board {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let mut ascii_repr: Vec<String> = vec![];

        ascii_repr.push(
            border::DOUBLE_NW.to_string()
                + (border::DOUBLE_HORIZONTAL.repeat(printing::CLIQUE_SIZE as usize)
                    + border::DOUBLE_SINGLE_S)
                    .repeat(2)
                    .as_str()
                + border::DOUBLE_HORIZONTAL
                    .repeat(printing::CLIQUE_SIZE as usize)
                    .as_str()
                + border::DOUBLE_NE
                + "\n",
        );

        for y1 in 0..(board::BOARD_HEIGHT / board::CLIQUE_HEIGHT) {
            for y2 in 0..board::CLIQUE_HEIGHT {
                let real_y = (y1 * board::CLIQUE_HEIGHT) + y2;

                ascii_repr.push(format!("{} ", border::DOUBLE_VERTICAL));
                for x1 in 0..(board::BOARD_WIDTH / board::CLIQUE_WIDTH) {
                    for x2 in 0..board::CLIQUE_WIDTH {
                        let real_x = (x1 * board::CLIQUE_WIDTH) + x2;

                        ascii_repr.push(format!(
                            "{} ",
                            self.raw_table[real_y as usize][real_x as usize]
                        ));
                    }

                    ascii_repr.push(format!("{} ", border::SINGLE_VERTICAL));
                }
                ascii_repr.pop().unwrap();
                ascii_repr.push(format!("{}\n", border::DOUBLE_VERTICAL));
            }

            if y1 != board::BOARD_HEIGHT / board::CLIQUE_HEIGHT - 1 {
                ascii_repr.push(
                    border::DOUBLE_SINGLE_E.to_string()
                        + (border::SINGLE_HORIZONTAL.repeat(printing::CLIQUE_SIZE as usize)
                            + border::SINGLE_CROSS)
                            .repeat(2)
                            .as_str()
                        + border::SINGLE_HORIZONTAL
                            .repeat(printing::CLIQUE_SIZE as usize)
                            .as_str()
                        + border::DOUBLE_SINGLE_W
                        + "\n",
                );
            }
        }

        ascii_repr.push(
            border::DOUBLE_SW.to_string()
                + (border::DOUBLE_HORIZONTAL.repeat(printing::CLIQUE_SIZE as usize)
                    + border::DOUBLE_SINGLE_N)
                    .repeat(2)
                    .as_str()
                + border::DOUBLE_HORIZONTAL
                    .repeat(printing::CLIQUE_SIZE as usize)
                    .as_str()
                + border::DOUBLE_SE,
        );

        f.write_str(ascii_repr.join("").as_str())
    }
}
