use crate::{board::*, consts, statistics::*};
use std::time::SystemTime;

pub trait BaseAlgorithm {
    fn solve(&mut self, board: &mut Board, print_func: Option<fn(&Board)>) -> Statistics;
    fn get_name(&self) -> &String;
}

pub struct BasicRecursiveBacktrackAlgorithm {
    pub num_assignments: i32,
    pub name: String,
}

impl BasicRecursiveBacktrackAlgorithm {
    pub fn new() -> Self {
        Self {
            num_assignments: 0,
            name: "BasicRecursiveBacktrackAlgorithm".to_string(),
        }
    }

    fn basic_recursive_backtrack_solve(
        &mut self,
        board: &mut Board,
        print_func: Option<fn(&Board)>,
        mut editable_index: Option<usize>,
    ) -> bool {
        match editable_index {
            None => editable_index = Some(0),
            _ => (),
        }

        let editable_index = Some(editable_index).unwrap().unwrap();

        if editable_index >= board.editable_indices.len() {
            return board.is_board_valid();
        }

        let (x, y) = board.editable_indices[editable_index];
        for attempt in 1..consts::board::BOARD_HEIGHT + 1 {
            board.raw_table[y as usize][x as usize] = attempt;

            if print_func.is_some() {
                (print_func.unwrap())(board);
            }

            self.num_assignments += 1;
            if !board.is_square_valid(editable_index) {
                continue;
            }

            let is_subtree_valid =
                self.basic_recursive_backtrack_solve(board, print_func, Some(editable_index + 1));

            if is_subtree_valid {
                return true;
            }
        }

        board.raw_table[y as usize][x as usize] = 0;
        self.num_assignments += 1;

        false
    }
}

impl BaseAlgorithm for BasicRecursiveBacktrackAlgorithm {
    fn solve(&mut self, board: &mut Board, print_func: Option<fn(&Board)>) -> Statistics {
        self.num_assignments = 0;

        let start = SystemTime::now();
        let result = self.basic_recursive_backtrack_solve(board, print_func, None);
        let elapsed = start.elapsed().unwrap().as_millis();

        return Statistics::new(result, self.num_assignments, elapsed);
    }

    fn get_name(&self) -> &String {
        &self.name
    }
}
