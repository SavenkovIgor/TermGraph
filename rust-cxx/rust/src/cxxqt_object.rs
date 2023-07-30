#[cxx_qt::bridge]
mod my_object {

    unsafe extern "C++" {
        include!("cxx-qt-lib/qstring.h");
        type QString = cxx_qt_lib::QString;
    }

    #[cxx_qt::qobject(qml_uri = "Cxx_qt_demo", qml_version = "1.0")]
    pub struct MyObject {
        #[qproperty]
        number: i32,
        #[qproperty]
        string: QString,
    }

    impl Default for MyObject {
        fn default() -> Self {
            Self {
                number: 0,
                string: QString::from(""),
            }
        }
    }

    impl qobject::MyObject {
        #[qinvokable]
        pub fn increment_number(self: Pin<&mut Self>) {
            let previous = *self.as_ref().number();
            self.set_number(previous + 1);
        }

        #[qinvokable]
        pub fn say_hi(&self, string: &QString, number: i32) {
            println!("Hi from Rust! String is {string}, and number is {number}");
        }
    }
}