[%bs.raw "require('flag-icon-css/css/flag-icon.css')"];

open Belt;

module Country = {
  type t = {
    label: string,
    value: string,
  };
  module Decode = {
    let countryDecoder = json =>
      Json.Decode.{
        label: json |> field("label", string),
        value: json |> field("value", string),
      };
    let make = json =>
      try(Result.Ok(json |> Json.Decode.array(countryDecoder))) {
      | Json.Decode.DecodeError(err) => Result.Error(err)
      };
  };
};

module MenuList = {
  let itemSize = 40;
  [@react.component]
  let make =
      (
        ~children,
        ~options: array(Country.t),
        ~maxHeight,
        ~getValue: unit => array(Js.Nullable.t(Country.t)),
      ) =>
    <ReactWindow.FixedSizeList
      itemSize
      height=maxHeight
      itemCount={options->Array.length}
      initialScrollOffset={
        options
        ->Array.getIndexBy(opt =>
            getValue()[0]
            ->Option.flatMap(o => o->Js.Nullable.toOption)
            ->Option.mapWithDefault(false, o => o === opt)
          )
        ->Option.getWithDefault(0)
        * itemSize
      }>
      {props =>
         <div style={props##style}>
           {children
            ->React.Children.toArray
            ->Array.get(props##index)
            ->Option.getWithDefault(React.null)}
         </div>}
    </ReactWindow.FixedSizeList>;
};

module Spread = {
  [@react.component]
  let make = (~props, ~children) =>
    ReasonReact.cloneElement(children, ~props, [||]);
};

module SelectOption = {
  let make = props =>
    <Spread props>
      <ReactSelect.Components.Option
        className={"flag-icon flag-icon-" ++ props##value}>
        {props##label->React.string}
      </ReactSelect.Components.Option>
    </Spread>;
};

let initialCountryData: WebData.t(array(Country.t)) = RemoteData.Loading;
let url = "https://gist.githubusercontent.com/rusty-key/659db3f4566df459bd59c8a53dc9f71f/raw/4127f9550ef063121c564025f6d27dceeb279623/counties.json";
[@react.component]
let make = () => {
  let (_country, setCountry) = React.useState(() => None);
  let (countryData, setCountryData) =
    React.useState(() => initialCountryData);

  React.useEffect0(() => {
    Fetch.fetch(url)
    |> WebData.fromResponse(Country.Decode.make)
    |> Js.Promise.then_(data => {
         setCountryData(_ => data);
         Js.Promise.resolve();
       })
    |> ignore;
    None;
  });

  switch (countryData) {
  | RemoteData.NotAsked
  | RemoteData.Loading => <ReactSelect isLoading=true isDisabled=true />
  | RemoteData.Success(cs) =>
    <ReactSelect
      options=cs
      placeholder="Select country"
      isSearchable=true
      onChange={x => setCountry(_ => Some(x))}
      components={menuList: MenuList.make, option: SelectOption.make}
    />
  | RemoteData.Failure(_) => React.null
  };
};