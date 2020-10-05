use crate::{algorithms::*, board::*, consts::*, statistics::*};
use std::{thread, time};

pub struct Solver;
impl Solver {
    pub fn slow_print(board: &Board) {
        thread::sleep(time::Duration::from_secs_f32(printing::SLEEP_DURATION));

        for _ in 0..printing::BOARD_HEIGHT {
            println!("\x1b[1A");
            println!("\x1b[2K");
        }

        println!("{}", board);
    }

    pub fn solve_board(
        board: &mut Board,
        mut algorithm: Box<dyn BaseAlgorithm>,
        do_slow_print: bool,
    ) -> Result<Statistics, String> {
        if !board.is_board_valid() {
            return Err("Board is initially invalid!".to_string());
        }

        println!("Solving with algorithm: {}", algorithm.get_name());
        println!("Board to solve:\n{}\n", board);

        if do_slow_print {
            for _ in 0..printing::BOARD_HEIGHT {
                println!();
            }
        }

        let statistics = algorithm.solve(
            board,
            if do_slow_print {
                println!("Preview:");
                Some(Self::slow_print)
            } else {
                None
            },
        );

        println!();
        if statistics.is_solved {
            println!("Solved board:\n{}", board);
        } else {
            return Err("Board is unsolvable!".to_string());
        }

        Ok(statistics)
    }
}
