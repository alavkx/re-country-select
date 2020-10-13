// type state = {isFocused: bool};
// type styles = {.};
// type styleFn = (styles, state) => styles;
// type customStyles = {dropdownIndicator: styleFn};
type components('a, 'b, 'c) = {
  [@bs.as "MenuList"]
  menuList: React.component('a),
  [@bs.as "Option"]
  option: React.component('c),
};
[@bs.module "react-select"] [@react.component]
external make:
  (
    ~classNamePrefix: string=?,
    ~isDisabled: bool=?,
    ~isLoading: bool=?,
    ~isMulti: bool=?,
    ~isSearchable: bool=?,
    ~name: string=?,
    ~onChange: unit => unit=?,
    ~options: array('a)=?,
    ~placeholder: string=?,
    ~noOptionsMessage: string => option(string)=?,
    ~value: string=?,
    ~components: components('b, 'c, 'd)=?
  ) =>
  React.element =
  "default";

module Components = {
  module Option = {
    [@bs.module "react-select"] [@bs.scope "components"] [@react.component]
    external make:
      (~children: React.element, ~className: string=?) => React.element =
      "Option";
  };
};