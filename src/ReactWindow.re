module FixedSizeList = {
  [@bs.module "react-window"] [@react.component]
  external make:
    (
      ~height: int,
      ~itemSize: int,
      ~itemCount: int,
      ~initialScrollOffset: int,
      ~children: {
                   .
                   "style": ReactDOMStyle.t,
                   "index": int,
                 } =>
                 React.element
    ) =>
    React.element =
    "FixedSizeList";
};