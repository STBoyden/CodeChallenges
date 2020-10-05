#[macro_use]
extern crate lazy_static;

mod algorithms;
mod board;
mod consts;
mod helpers;
mod solver;
mod statistics;

use crate::{algorithms::*, board::*, helpers::*, solver::*};
use std::{env, fs::*, io::prelude::*, io::BufReader, path::Path};

fn main() {
    let usage_error =
        "Usage: cargo run --release -- [INPUT FILE] [ALGORITHM INDEX] [VERBOSE LEVEL]";

    let args: Vec<String> = env::args().collect();

    if args.len() < 4 {
        eprintln!("{}", usage_error);
        return;
    }

    let algorithm_index: i32 = match args[2].parse() {
        Ok(v) => v,
        Err(_) => {
            eprintln!("{}", usage_error);
            eprintln!("[ALGORITHM INDEX] must be a valid integer between the ranges 0..0.");
            return;
        }
    };

    let algorithm: Box<dyn BaseAlgorithm> = Box::new(match algorithm_index {
        0 => BasicRecursiveBacktrackAlgorithm::new(),
        _ => {
            eprintln!("Invalid index for algorithm. Please select from range of 0..0 ");
            return;
        }
    });

    let verbose_level: i32 = match args[3].parse() {
        Ok(v) => v,
        Err(_) => {
            eprintln!("{}", usage_error);
            eprintln!("[VERBOSE LEVEL] must be either 1 or 0.");

            return;
        }
    };

    let file_name = args[1].clone();
    if !Path::new(&file_name).exists() {
        eprintln!("{}", usage_error);
        eprintln!("{} is not a valid path!", file_name);

        return;
    }

    let mut board_raw_list = vec![];
    let file = File::open(file_name).unwrap();
    let file = BufReader::new(file);
    for line in file.lines() {
        let line = line.unwrap().clone();
        let line = line.trim_end();

        let row: Vec<i32> = line.split(" ").map(|b| b.parse::<i32>().unwrap()).collect();
        if !row.len() == consts::board::BOARD_WIDTH as usize {
            eprintln!("Unable to parse file!");
            return;
        }

        board_raw_list.push(row);
    }

    BOARDHELPER.lock().unwrap().init();
    let mut board = Board::new(&board_raw_list).unwrap();
    let statistics = Solver::solve_board(
        &mut board,
        algorithm,
        match verbose_level {
            1 => true,
            _ => false,
        },
    )
    .unwrap();

    println!("{}", statistics);
}
