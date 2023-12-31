#[test]
fn test_123() {
    use crate::{run, Mode};
    run(Mode::Debug, "1-1");
    run(Mode::Debug, "2-2");
    run(Mode::Debug, "2-3");
}

#[test]
fn test_3_4() {
    use crate::{get_data, run_with_stdin, Mode};
    let input = get_data();
    run_with_stdin(Mode::Release, "3-4", input);
}
