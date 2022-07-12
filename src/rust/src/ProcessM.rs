#![allow(non_snake_case)]

use colored::Colorize;

pub struct Process
{
    name: String,
    args: Vec<String>,
    info: String,
}

impl Process
{
    pub fn Init(name: &str, args: &str) -> Process
    {
        Process::InitInfo(name, args, "")
    }

    pub fn InitInfo(name: &str, args: &str, info: &str) -> Process
    {
        let argsVec: Vec<String> = args.split_whitespace().map(str::to_string).collect();
        Process { name: String::from(name), args: argsVec, info: String::from(info) }
    }

    pub fn Exec(&self)
    {
        let mut s=String::new();
        println!();
        if self.info.chars().count() > 0
        {
            println!("{}", format!("[Arch install script]<Info>: {}", self.info).blue());
            println!();
        }

        println!("{}", format!("[Arch install script]<Executing program>: {} {:?}", self.name, self.args).green());
        let child = std::process::Command::new(&self.name).args(&self.args).status();

        if child.is_ok() {
            println!("{}", format!("[Arch install script]<Finished executing program>: {}", self.name).green());
        }
        else {
            println!("{}", format!("[Arch install script]<Finished executing program (Unsuccessfull)>: {}", self.name).red());
            println!("{:?}", child.unwrap_err());
        }
        std::io::stdin().read_line(&mut s).expect("Did not enter a correct string");
    }
}
